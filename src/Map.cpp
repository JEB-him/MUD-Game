/**
 * @file Map.cpp
 * @author Jie Jiang
 */
#include "Map.h"
#include "Controller.h"
#include <fstream>
#include <algorithm>
#include <filesystem>

Map::Map(const std::string& filename, const Position& pos):
    modified(false), map(), x(-1), y(-1) {
    // 这个地方被搞到了，由于我是在测试中写了很多次 Map，而释放 Map 再创建一个
    // Map 的对象时，C++ 让 map 数组重新使用了原来的内存区域，巧合的导致了一些
    // 没有赋值的地方储存了旧的垃圾值，导致程序出现了异常判断，因此需要在初始
    // 化列表中对 map 进行默认初始化
    Message load_msg = loadMap(filename);
    if (load_msg.status) {
        this->is_valid = false;
        this->valid_msg = load_msg.msg;
    } else {
        this->is_valid = true;
        this->valid_msg = "";
    }
    if (x == -1) {
        this->x = pos.x;
        this->y = pos.y;
    }
}

Map::~Map() {
    // 析构函数只是用来兜底的，Controller 应当自行 save 以获取信息
    if (is_valid && modified) {
        Message success = save();
        // TODO 添加日志输出
    }
}

bool Map::valid() const {
    return this->is_valid;
}

const std::string& Map::getValidMsg() const {
    return valid_msg;
}


Position Map::getPos() const {
    return {x, y};
}

int Map::getMaxWidth() const {
    return max_width;
}

int Map::getMaxHeight() const {
    return max_height;
}

/**
 * @details 移动主角有下面的事项需要注意：
 *          1. 主角不能进入宽度小于其 width 的空间（着重注意其他宽字符）
 *          2. 主角碰撞完器械、NPC、墙壁以及进入出口后，位置不会发生改变
 */
Message Map::moveProtagonist(const int &direction, EventType &event_type, int &id)
{
    if (y + 1 >= MAX_WIDTH) {
        return {"Bad position", -1};
    }
    char back_code = detectCollision({x + DIRECTIONS[direction][0], y + DIRECTIONS[direction][1]});
    switch (back_code) {
        case -2:    // 墙壁/空间狭小
            event_type = EventType::NONE;
            id = -1;
            return {"不可通行：墙壁/空间狭小", 1};
            break;
        case -1:    // 普通移动
            map[x][y] = ' ';
            x += DIRECTIONS[direction][0];
            y += DIRECTIONS[direction][1];
            map[x][y] = '1';
        case 'i':
            event_type = EventType::NONE;
            id = -1;
            return {"Success", 0};
        case 'o':
            event_type = EventType::JUMP;
            id = getExitId({x + DIRECTIONS[direction][0], y + DIRECTIONS[direction][1]});
            return {"抵达出口", 0};
        case '9':
            event_type = EventType::AC_NPC;
            id = getNPCId({x + DIRECTIONS[direction][0], y + DIRECTIONS[direction][1]});
            return {"与 NPC 交互", 0};
        default:
            event_type = EventType::AC_INST;
            id = char2index(back_code);
            return {"与器械交互", 0};
    }
    return {"", 0};
}

Message Map::save() const {
    std::ofstream map_file(map_path.c_str());
    if (!map_file.is_open()){
        return {"无法打开: " + map_path, -1};
    }

    for (int i = 0; i < MAX_HEIGHT; ++i) {
        for (int j = 0; j < MAX_WIDTH; ++j) {
            if (map[i][j]) {
                map_file << map[i][j];
            }
        }
        map_file << std::endl;
    }
    auto controller = Controller::getInstance();
    controller->log(Controller::LogLevel::DEBUG, map_path);

    return {"Success", 0};
}

char Map::detectCollision(const Position& pos) const {
    /* 检查是否碰壁或空间狭小 */
    if (map[pos.x][pos.y] == '#' || map[pos.x][pos.y + 1] == '#') return -2;

    /* 判断是否有任何宽字符 */
    for (int i = 0, st, ed; i < CHAR_MAXN; ++ i) {
        if (i == 1) continue;
        if (i == char2index('o')) continue;
        if (i == char2index('i')) continue;
        st = pos.y - SPECIAL_CHARS[i].width + 1;
        st = std::max(st, 0 );
        ed = std::min(pos.y + SPECIAL_CHARS[1].width - 1, MAX_WIDTH);
        while (st <= ed) {
            if (char2index(map[pos.x][st]) == i) {
                return map[pos.x][st];
            }
            ++st;
        }
    }
    if (pos.x != x) {
        /* 检查是否到横的exit/entry */
        for (int i = std::max(0, pos.y - 3); i <= std::min(MAX_WIDTH, pos.y + SPECIAL_CHARS[1].width - 1); ++i) {
            if (map[pos.x][i] == 'o') return 'o';
            if (map[pos.x][i] == 'i') return 'i';
        }
    } else {
        /* 检查是否到竖的exit/entry */
        for (int i = std::max(0, pos.x - 1); i <= pos.x; ++i) {
            if (map[i][pos.y] == 'o' || map[i][pos.y + 1] == 'o') return 'o';
            if (map[i][pos.y] == 'i' || map[i][pos.y + 1] == 'i') return 'i';
        }
    }

    return -1;
}


Map::LineType Map::classifyLine(const std::string& line) {
    bool filled = true;

    if (line.length() > MAX_WIDTH) return LineType::OVER_SIZE;
    // 检查是否为空行
    int num = 0;
    for (int i = 0; i < line.length(); ++i) {
        const auto& ch = line[i];
        if (ch == '#') {
            ++ num;
            max_width = i + 1;
        } else if (ch == '\r') {
            /* Maybe some bad chars were typed in Windows. */
            return LineType::INVAILD_LINE;
        } else if (ch == '\n') {
            /* 这种情况是不应该发生的，除非你写了诡异的代码 */
            return LineType::INVAILD_LINE;
        }
    }
    if (num == 0) {
        if (!is_empty) {
            return LineType::WALL;
        } else {
            return LineType::EMPTY_LINE;
        }
    }

    is_empty = false;
    return LineType::WALL;
}

Message Map::loadMap(const std::string& filename) {
    // 检查文件路径
    for (const auto& ch : filename)
        if (ch == '/' || ch == '\\') return {"非法文件名", -1};
    map_path = BASE_DIR + filename;
    bool return_is_valid = false;

    // 获取当前路径
    auto current_path = std::filesystem::current_path();
    // 为了更安全的打开，需要将 map_path 转换成 c 风格的字符串
    std::ifstream map_file(map_path.c_str());
    if (!map_file.is_open()){
        return {"无法打开: " + map_path, -1};
    }
    std::string line;
    // 清空空白符号
    map_file >> std::ws;
    // 当 file 读到 EOF 时，eof_bit 会被置为1（好像是）, 这时(bool)file 也是 false
    // 当 file 读取到的字符串超出 str.max_size() 或没有读取到任何字符时，会将 fail 置为 1
    int rows = 0;
    while(std::getline(map_file, line)) {
        // 读取行
        LineType line_type = classifyLine(line);
        switch (line_type) {
            case LineType::WALL:
                if (rows >= MAX_HEIGHT) return {"尺寸超出最大限制", -1};
                if (!line_copy(map[rows], line)) {
                    return {"无效地图：含有非法字符", -1};
                }
                break;
            case LineType::EMPTY_LINE:
                --rows;
                break;
            case LineType::INVAILD_LINE:
                return {"在classifyLine 中检测出：无效地图", -1};
            case LineType::OVER_SIZE:
                return {"尺寸超出最大限制", -1};
            default:
                return {"未知异常：接收到 getline 返回的异常值", -1};
        }
        ++rows;
        if (rows > MAX_HEIGHT)
            return {"尺寸超出最大限制", -1};
    }
    max_height = 0;
    for (int i = rows - 1; i >= 0 && !max_height; --i) {
        for (int j = 0; j < MAX_WIDTH; ++j) {
            if (map[i][j] == '#') {
                max_height = i + 1;
            }
        }
    }
    if (is_empty)
        return {"无效地图", -1};
    if (map_file.fail() && !map_file.eof())
        return {"读取错误", -1};

    bool approved = processMap();
    if (approved) {
        // 设置出口和 NPC 的 Id
        if (!indexInit(rows)) {
            return {"地图索引建立，主角生成失败", -1};
        }
        return {"", 0};
    } else return {"地图未封闭，宽字符放置不合理或未知异常", -1};
}

bool Map::indexInit(const int& rows) {
    int times = std::min(MAX_HEIGHT, rows + 1);
    for (int i = 0; i < times; ++i) {
        for (int j = 0;j < MAX_WIDTH; ++ j) {
            if (!map[i][j]) break;
            if (map[i][j] == 'o') {
                // TODO 添加出口 ID
            } else if (map[i][j] == 'i') {
                // TODO 添加入口 ID
            } else if (map[i][j] == '9') {
                // TODO 添加 NPC ID

            } else if (map[i][j] == '1') {
                if (x != -1 || y != -1) {
                    return false;
                }
                x = i;
                y = j;
            }
        }
    }
    return true;
}

bool Map::line_copy(char map_line[], const std::string& line) {
    int len = std::min(MAX_WIDTH, static_cast<int>(line.length()));
    int i = 0;
    for (; i < len; ++i) {
        if ('0' <= line[i] && line[i] <= '9' ||
            'a' <= line[i] && line[i] <= 'z' ||
            'A' <= line[i] && line[i] <= 'Z' ||
            line[i] == '#' || line[i] == ' ') {
            map_line[i] = line[i];
        } else {
            return false;
        }
    }
    for (; i < MAX_WIDTH; ++i) {
        map_line[i] = ' ';
    }
    return true;
}

bool Map::processMap() {
    int sx = -1, sy = -1;
    for (int i = 0; sx == -1 && i < MAX_HEIGHT; ++i)
        for (int j = 0; j < MAX_WIDTH; ++j)
            if (map[i][j] == '#') {
                sx = i, sy = j;
                break;
            }
    if (sx == -1 || sy == -1) {
        return false;
    }

    int n = MAX_WIDTH * MAX_HEIGHT;
    int x = sx, y = sy;
    calcDir(x, y, true);
    while(n--) {
        if (x == sx && y == sy) return true;
        if (!calcDir(x, y)) return false;
    }
    return false;
}

bool Map::calcDir(int& x, int& y, const bool& reset) {
    /* 代码逻辑混乱 orz，非必要勿动 */
    static int horizon;       // 正为右
    static int vertical;      // 正为下
    if (reset) {
        horizon = 1;
        vertical = 0;
    }

    // 检查宽字符
    if (vertical) {
        if(!checkWideChar(x, y)) return false;
    }

    /* 按原有方向走 */
    if (horizon && y + horizon >= 0 && y + horizon < MAX_WIDTH) {
        // 无出口或入口
        if (map[x][y + horizon] == '#') {
            y += horizon;
            return true;
        } else {
            bool flag = true;
            int ty = y + horizon * (horizon + 5) % 5;
            if (map[x][ty] == 'o' || map[x][ty] == 'i') {
                for (int i = horizon * ((horizon * 2 + 5) % 5); i != 5 && i != 0; ++i) {
                    ty = y + i;
                    if (ty < 0 ||
                        ty >= MAX_WIDTH ||
                        map[x][ty] != ' ') {
                        flag = false;
                        break;
                    }
                }
            } else flag = false;
            if (flag) {
                y += 4 * horizon;
                return true;
            }
        }
    }
    if (vertical && x + vertical >= 0 && x + vertical < MAX_HEIGHT) {
        // 无出口或入口
        if (map[x + vertical][y] == '#') {
            x += vertical;
            return true;
        } else {
            bool flag = true;
            int tx = x + vertical * (vertical + 3) % 3;
            if (map[tx][y] == 'o' || map[tx][y] == 'i') {
                if(!checkWideChar(tx, y)) {
                    return false;
                }
                for (int i = vertical * ((vertical * 2 + 3) % 3); i != 3 && i != 0; ++i) {
                    tx = x + i;
                    if (tx < 0 ||
                        tx >= MAX_WIDTH ||
                        map[tx][y] != ' ') {
                        flag = false;
                        break;
                    } else if(!checkWideChar(tx, y)) {
                        return false;
                    }
                }
            } else flag = false;
            if (flag) {
                x += 2 * vertical;
                return true;
            }
        }
    }

    // 换方向
    if (horizon) {
        vertical = 0;
        if (x >= 1 && map[x - 1][y] == '#') {
            vertical -= 1;
        }
        if (x >= 2 && (map[x - 2][y] == 'i' || map[x - 2][y] == 'o')) {
            vertical -= 1;
        }
        if (vertical < -1) return false;
        if (x + 1 < MAX_HEIGHT && (map[x + 1][y] == 'i' || map[x + 1][y] == 'o' || map[x + 1][y] == '#')) {
            vertical += 1;
        }
        if (!vertical) return false;
        horizon = 0;
        if (map[x + vertical][y] == '#') {
            x += vertical;
        }
        return true;
    }

    if (vertical) {
        horizon = 0;
        if (y >= 1 && map[x][y - 1] == '#') {
            horizon -= 1;
        }
        if (y >= 4 && (map[x][y - 4] == 'i' || map[x][y - 4] == 'o')) {
            horizon -= 1;
        }
        if (horizon < -1) return false;
        if (y + 1 < MAX_WIDTH && (map[x][y + 1] == 'i' || map[x][y + 1] == 'o' || map[x][y + 1] == '#')) {
            horizon += 1;
        }
        if (!horizon) return false;
        vertical = 0;
        if (map[x][y + horizon] == '#') {
            y += horizon;
        }
        return true;
    }

    return false;
}

int Map::char2index(const char& ch) {
    if ('0' <= ch && ch <= '9')
        return ch - '0';
    else if ('a' <= ch && ch <= 'z')
        return 10 + ch - 'a';
    else if ('A' <= ch && ch <= 'Z')
        return 36 +  + ch - 'a';

    return -1;
}

bool Map::checkWideChar(const int& x, const int& y) {
    int index = -2;
    if (map[x][y - 1] != ' ' && map[x][y - 1] != '#') {
        index = char2index(map[x][y - 1]);
    }
    if (map[x][y + 1] != ' ' && map[x][y + 1] != '#') {
        index = char2index(map[x][y + 1]);
    }
    if (index != -2 && (index == -1 || SPECIAL_CHARS[index].width > 1)) return false;
    return true;
}

int Map::getExitId(const Position& pos) {
    // TODO: 等待场景类实现
    return -1;
}

int Map::getNPCId(const Position& pos) {
    // TODO: 等待 NPC 类实现
    return -1;
}
