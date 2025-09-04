/**
 * @file Map.cpp
 * @author Jie Jiang
 */
#include "Map.h"
#include <fstream>
#include <algorithm>
#include <filesystem>

Map::Map(const std::string& filename, const Position& pos):
    modified(false) {
    Message load_msg = loadMap(filename);
    if (load_msg.status) {
        this->is_valid = false;
        this->valid_msg = load_msg.msg;
    } else {
        this->is_valid = true;
        this->valid_msg = "";
    }
    this->x = pos.x;
    this->y = pos.y;
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

Message Map::moveProtagonist(const int& direction, int& event_type, int& id) {
    
    return {"", 0};
}

Message Map::save() const {
    return {"", 0};
}

Map::LineType Map::classifyLine(const std::string& line) {
    bool filled = true;

    if (line.length() > MAX_WIDTH) return LineType::OVER_SIZE;

    // 检查是否为空行
    int num = 0;
    for (auto& ch : line) {
        if (ch == '#') {
            ++ num;
        } else if (ch == '\r') {
            /* Maybe some bad chars were typed in Windows. */
            return LineType::INVAILD_LINE;
        } else if (ch == '\n') {
            /* 这种情况是不应该发生的，除非你写了诡异的代码 */
            return LineType::INVAILD_LINE;
        }
    }
    if (num == 0) {
        if (checked.top && !checked.bottom) {
            checked.middle = true;
            return LineType::MIDDLE_LINE_OF_WALL;
        } else {
            return LineType::EMPTY_LINE;
        }
    } else if (num == 1) {
        if (checked.top == checked.bottom) {
            return LineType::INVAILD_LINE;
        }
    }

    // 检查是否为一个顶部或底部的墙壁
    int len = 0;
    for (int i = line.length(); i > 0; --i)
        if (line[i - 1] == '#') {
            len = i;
            break;
        }

    int i = 0;
    for (; i < len; ++i) {
        const char& ch = line[i];
        if (ch == '#') {
            ++i;
            break;
        } else if (ch != ' ') {
            return LineType::INVAILD_LINE;
        }
    }
    if (i == len) return LineType::MIDDLE_LINE_OF_WALL;
    for (; i < len; ++ i) {
        const char& ch = line[i];
        if (ch != '#') {
            if (i - 1 >= 0 && line[i - 1] != '#') {
                filled = false;
                break;
            } else {
                int cmp_res = line.compare(i, 4, "o   ");
                if(cmp_res) cmp_res = line.compare(i, 4, "i   ");
                if(cmp_res) filled = false;
                else {
                    i += 3;
                }
            }
        }
    }

    if (filled) {
        if (checked.top) {
            checked.bottom = true;
            return LineType::BOTTOM_OF_WALL;
        } else if (checked.bottom) {
            return LineType::INVAILD_LINE;
        } else {
            checked.top = true;
            return LineType::TOP_OF_WALL;
        }
    }

    if (!checked.top) return LineType::INVAILD_LINE;
    checked.middle = true;
    return LineType::MIDDLE_LINE_OF_WALL;
}

Message Map::loadMap(const std::string& filename) {
    // TODO: 检查文件路径
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
            case LineType::TOP_OF_WALL:
            case LineType::BOTTOM_OF_WALL:
            case LineType::MIDDLE_LINE_OF_WALL:
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
    if (!checked.top || !checked.middle || !checked.bottom)
        return {"无效地图", -1};
    if (map_file.fail() && !map_file.eof())
        return {"读取错误", -1};

    bool approved = processMap();
    if (approved)
        return {"", 0};
    else
        return {"地图未封闭，宽字符放置不合理或未知异常", -1};
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
    /* 代码逻辑复杂，非必要勿动 */
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
            if (map[x][ty] == 'o' || map[x][ty] == 'i')
                for (int i = horizon * ((horizon * 2 + 5) % 5); i != 5 && i != 0; ++i) {
                    ty = y + i;
                    if (ty < 0 ||
                        ty >= MAX_WIDTH ||
                        map[x][ty] != ' ') {
                        flag = false;
                        break;
                    }
                }
            else flag = false;
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
