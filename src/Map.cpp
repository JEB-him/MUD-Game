// /**
//  * @file Map.cpp
//  * @author Jie Jiang
//  */
// #include "Map.h"
// #include <fstream>
// #include <algorithm>
// 
// Map::Map(const std::string& map_path, const Position& pos):
//     map_path(map_path),
//     modified(false) {
//     Message load_msg = loadMap();
//     if (load_msg.status) {
//         this->is_valid = false;
//         this->valid_msg = load_msg.msg;
//     } else {
//         this->is_valid = true;
//         this->valid_msg = "";
//     }
//     this->x = pos.x;
//     this->y = pos.y;
//     this->map_path = std::move(map_path);
// }
// 
// Map::~Map() {
//     // 析构函数只是用来兜底的，Controller 应当自行 save 以获取信息
//     if (is_valid && modified) {
//         Message success = save();
//         // TODO 添加日志输出
//     }
// }
// 
// bool Map::valid() const {
//     return this->is_valid;
// }
// 
// const std::string& Map::getValidMsg() const {
//     return valid_msg;
// }
// 
// 
// Position Map::getPos() const {
//     return {x, y};
// }
// 
// Message Map::moveProtagonist(const int& direction, int& event_type, int& id) {
//     
//     return {"", 0};
// }
// 
// Map::LineType Map::classifyLine(const std::string& line) {
//     bool filled = true;
// 
//     if (line.length() > MAX_WIDTH) return LineType::OVER_SIZE;
// 
//     // 检查是否为空行
//     int wall_num = 0;
//     for (auto& ch : line) {
//         if (ch != ' ') {
//             ++ wall_num;
//         } else if (ch == '\r') {
//             return LineType::INVAILD_LINE;
//         } else if (ch == '\n') {
//             /* 这种情况是不应该发生的，除非你写了诡异的代码 */
//             return LineType::INVAILD_LINE;
//         }
//     }
//     if (wall_num == 2) {
//         return LineType::MIDDLE_LINE_OF_WALL;
//     } else if (wall_num == 1) {
//         if (checked.top && !checked.middle) {
//             return LineType::MIDDLE_LINE_OF_WALL;
//         } else {
//             return LineType::INVAILD_LINE;
//         }
//     } else if (wall_num == 0) {
//         if (checked.top && !checked.middle) {
//             return LineType::MIDDLE_LINE_OF_WALL;
//         } else {
//             return LineType::EMPTY_LINE;
//         }
//     }
// 
//     // 检查是否为一个顶部或底部的墙壁
//     for (int i = 0; i < line.length(); ++ i) {
//         const char& ch = line[i];
//         if (ch != '#') {
//             if (i - 1 >= 0 && line[i - 1] != '#') {
//                 filled = false;
//                 break;
//             } else {
//                 int cmp_res = line.compare(i, 4, "o   ");
//                 if(cmp_res) cmp_res = line.compare(i, 4, "i   ");
//                 if(cmp_res) filled = false;
//             }
//         }
//     }
// 
//     if (filled) {
//         if (checked.top) {
//             return LineType::BOTTOM_OF_WALL;
//         } else if (checked.bottom) {
//             return LineType::INVAILD_LINE;
//         } else {
//             return LineType::TOP_OF_WALL;
//         }
//     }
// 
//     return LineType::INVAILD_LINE;
// }
// 
// Message Map::loadMap() {
//     bool return_is_valid = false;
//     // 为了更安全的打开，需要将 map_path 转换成 c 风格的字符串
//     std::ifstream map_file(map_path.c_str());
//     if (!map_file.is_open()){
//         return {"无法打开: "+map_path, -1};
//     }
//     std::string line;
//     // 清空空白符号
//     map_file >> std::ws;
//     // 当 file 读到 EOF 时，eof_bit 会被置为1（好像是）, 这时(bool)file 也是 false
//     // 当 file 读取到的字符串超出 str.max_size() 或没有读取到任何字符时，会将 fail 置为 1
//     int rows = 0;
//     while(std::getline(map_file, line)) {
//         // 读取行
//         LineType line_type = classifyLine(line);
//         switch (line_type) {
//             case LineType::TOP_OF_WALL:
//             case LineType::BOTTOM_OF_WALL:
//             case LineType::MIDDLE_LINE_OF_WALL:
//                 if (!line_copy(map[rows], line)) {
//                     return {"无效地图：含有非法字符", -1};
//                 }
//                 break;
//             case LineType::EMPTY_LINE:
//                 --rows;
//                 break;
//             case LineType::INVAILD_LINE:
//                 return {"无效地图", -1};
//             case LineType::OVER_SIZE:
//                 return {"尺寸超出最大限制", -1};
//             default:
//                 return {"未知异常：接收到 getline 返回的异常值", -1};
//         }
//         ++rows;
//         if (rows > MAX_HEIGHT)
//             return {"尺寸超出最大限制", -1};
//     }
//     if (!checked.top || !checked.middle || !checked.bottom)
//         return {"无效地图", -1};
//     if (map_file.fail())
//         return {"读取错误", -1};
// 
//     bool approved = processMap();
//     if (approved)
//         return {"Success", 0};
//     else
//         return {"地图未封闭，宽字符放置不合理或未知异常", 0};
// }
// 
// bool Map::line_copy(char map_line[], const std::string& line) {
//     int len = std::min(MAX_WIDTH, static_cast<int>(line.length()));
//     int i = 0;
//     for (; i < len; ++i) {
//         if ('0' <= line[i] && line[i] <= '9' ||
//             'a' <= line[i] && line[i] <= 'z' ||
//             'A' <= line[i] && line[i] <= 'Z' ||
//             line[i] == '#') {
//             map_line[i] = line[i];
//         } else {
//             return false;
//         }
//     }
//     for (; i < MAX_WIDTH; ++i) {
//         map_line[i] = ' ';
//     }
//     return true;
// }
// 
// bool Map::processMap() {
//     int sx = -1, sy = -1;
//     for (int i = 0; i < MAX_HEIGHT; ++i)
//         for (int j = 0; j < MAX_WIDTH; ++j)
//             if (map[i][j] == '#') {
//                 sx = i, sy = j;
//                 break;
//             }
//     if (sx == -1 || sy == -1) {
//         return false;
//     }
// 
//     int n = MAX_WIDTH * MAX_HEIGHT;
//     while(n--) {
// 
//     }
//     return false;
// }
// 
// int Map::calcDir(int& x, int& y, const bool& reset=0) {
//     static int horizon;       // 正为右
//     static int vertical;      // 正为下
//     if (reset) {
//         horizon = 1;
//         vertical = 0;
//     }
// 
//     // 按原有方向走
//     if (horizon && y + horizon >= 0 && map[x][y + horizon] == '#') {
//         
//         return 0;
//     }
//     if (vertical && x + vertical >= 0 && map[y][x + vertical] == '#') {
// 
//     }
//     return 0;
// }
// 
// int Map::char2index(const char& ch) {
//     if ('0' <= ch && ch <= '9')
//         return ch - '0';
//     else if ('a' <= ch && ch <= 'z')
//         return 10 + ch - 'a';
//     else if ('A' <= ch && ch <= 'Z')
//         return 36 +  + ch - 'a';
// 
//     return -1;
// }
