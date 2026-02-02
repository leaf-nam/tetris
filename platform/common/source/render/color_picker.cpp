#include "render/color_picker.hpp"

#include <random>
#include <string>
#include <wtypes.h>

using namespace std;

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<int> dist(static_cast<int>(Color::CYAN),
                                               static_cast<int>(Color::YELLOW));
string ColorPicker::get_block_color(int type) { return get_color(get_color_key(type)).c_str(); }

string ColorPicker::get_block_color(const Tetromino& tetromino)
{
    return get_color(get_color_key(tetromino));
}

Color ColorPicker::get_color_key(const Tetromino& tetromino)
{
    return get_color_key(tetromino.get_mino_type());
}

Color ColorPicker::get_color_key(int type)
{
    switch (type) {
    case 0:
        return Color::CYAN;
    case 1: // O 미노 (Yellow - 노란색)
        return Color::ORANGE;
    case 2: // Z 미노 (Red - 빨간색)
        return Color::RED;
    case 3: // S 미노 (Green - 초록색)
        return Color::GREEN;
    case 4: // J 미노 (Blue - 파란색)
        return Color::PINK;
    case 5: // L 미노 (Orange -> 터미널은 보통 흰색 or 밝은 노랑으로 대체)
        return Color::YELLOW;
    case 6: // T 미노 (Purple - 보라색)
        return Color::PURPLE;
    case 7: // 방해 블록 (Gray - 회색)
        return Color::COMMENT;
    default: // 예외 (Reset)
        return Color::BACKGROUND;
    }
}

Color ColorPicker::get_random_color() { return static_cast<Color>(dist(gen)); }
