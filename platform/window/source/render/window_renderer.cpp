#include "render/window_renderer.hpp"

#include <conio.h>
#include <random>
#include <string>
#include <wtypes.h>

bool is_single = false;
int logo_x = (is_single) ? 20 : 2;
int board_x = (is_single) ? 35 : 25;
int middle_x = (is_single) ? 80 : 58;

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<int> dist(static_cast<int>(ColorKey::CYAN),
                                               static_cast<int>(ColorKey::YELLOW));
namespace color {
const char* const RESET = "\x1b[0m";
const char* const RED = "\x1b[31m";
const char* const GREEN = "\x1b[32m";
const char* const YELLOW = "\x1b[33m";
const char* const CYAN = "\x1b[36m";
const char* const PURPLE = "\x1b[35m";
const char* const GRAY = "\x1b[90m";
const char* const BOLD = "\x1b[1m";
} // namespace color

const char* const BIG_T[] = {"█████", "  █  ", "  █  ", "  █  ", "  █  "};
const char* const BIG_E[] = {"█████", "█    ", "███  ", "█    ", "█████"};
const char* const BIG_R[] = {"████ ", "█   █", "████ ", "█  █ ", "█   █"};
const char* const BIG_I[] = {"███", " █ ", " █ ", " █ ", "███"};
const char* const BIG_S[] = {" ████", "█    ", " ███ ", "    █", "████ "};
const char* const BIG_N[] = {"██  █", "█ █ █", "█  ██", "█   █", "█   █"};
const char* const BIG_M[] = {"█   █", "██ ██", "█ █ █", "█ █ █", "█   █"};
const char* const BIG_Y[] = {"█   █", "█   █", " █ █ ", "  █  ", "  █  "};
const char* const BIG_G[] = {" ███ ", "█    ", "█  ██", "█   █", " ████"};

constexpr const char* BIG_FONT[26][5] = {
    // A
    {" ███ ", "█   █", "█████", "█   █", "█   █"},
    // B
    {"████ ", "█   █", "████ ", "█   █", "████ "},
    // C
    {" ████", "█    ", "█    ", "█    ", " ████"},
    // D
    {"████ ", "█   █", "█   █", "█   █", "████ "},
    // E
    {"█████", "█    ", "███  ", "█    ", "█████"},
    // F
    {"█████", "█    ", "███  ", "█    ", "█    "},
    // G
    {" ████", "█    ", "█  ██", "█   █", " ████"},
    // H
    {"█   █", "█   █", "█████", "█   █", "█   █"},
    // I
    {"███", " █ ", " █ ", " █ ", "███"},
    // J
    {"  ███", "   █ ", "   █ ", "█  █ ", " ██  "},
    // K
    {"█   █", "█  █ ", "███  ", "█  █ ", "█   █"},
    // L
    {"█    ", "█    ", "█    ", "█    ", "█████"},
    // M
    {"█   █", "██ ██", "█ █ █", "█   █", "█   █"},
    // N
    {"█   █", "██  █", "█ █ █", "█  ██", "█   █"},
    // O
    {" ███ ", "█   █", "█   █", "█   █", " ███ "},
    // P
    {"████ ", "█   █", "████ ", "█    ", "█    "},
    // Q
    {" ███ ", "█   █", "█ █ █", "█  ██", " ████"},
    // R
    {"████ ", "█   █", "████ ", "█  █ ", "█   █"},
    // S
    {" ████", "█    ", " ███ ", "    █", "████ "},
    // T
    {"█████", "  █  ", "  █  ", "  █  ", "  █  "},
    // U
    {"█   █", "█   █", "█   █", "█   █", " ███ "},
    // V
    {"█   █", "█   █", "█   █", " █ █ ", "  █  "},
    // W
    {"█   █", "█   █", "█ █ █", "██ ██", "█   █"},
    // X
    {"█   █", " █ █ ", "  █  ", " █ █ ", "█   █"},
    // Y
    {"█   █", " █ █ ", "  █  ", "  █  ", "  █  "},
    // Z
    {"█████", "   █ ", "  █  ", " █   ", "█████"}};

using namespace std;
using namespace color;

const char* WindowRenderer::get_block_color(int type)
{
    return get_color(get_color_key(type)).c_str();
}

string WindowRenderer::get_block_color(const Tetromino& tetromino)
{
    return get_color(get_color_key(tetromino));
}

ColorKey WindowRenderer::get_color_key(const Tetromino& tetromino)
{
    return get_color_key(tetromino.get_mino_type());
}

ColorKey WindowRenderer::get_color_key(int type)
{
    switch (type) {
    case 0:
        return ColorKey::CYAN;
    case 1: // O 미노 (Yellow - 노란색)
        return ColorKey::ORANGE;
    case 2: // Z 미노 (Red - 빨간색)
        return ColorKey::RED;
    case 3: // S 미노 (Green - 초록색)
        return ColorKey::GREEN;
    case 4: // J 미노 (Blue - 파란색)
        return ColorKey::PINK;
    case 5: // L 미노 (Orange -> 터미널은 보통 흰색 or 밝은 노랑으로 대체)
        return ColorKey::YELLOW;
    case 6: // T 미노 (Purple - 보라색)
        return ColorKey::PURPLE;
    case 7: // 방해 블록 (Gray - 회색)
        return ColorKey::COMMENT;
    default: // 예외 (Reset)
        return ColorKey::BACKGROUND;
    }
}

void WindowRenderer::set_cursor(int x, int y) { printf("\033[%d;%dH", y + 1, x + 1); }

void WindowRenderer::draw_logo(int x, int y)
{
    print_big_string({x, y}, "TETRISSEN");
    print_small_string({x + 56, y + 4}, "v1");
}

void WindowRenderer::draw_enemy_title(int x, int y)
{
    for (int i = 0; i < 5; i++) {
        set_cursor(78, y + i);
        // 순서대로 색상과 함께 출력

        printf("%s%s ", GRAY, BIG_E[i]); // E
        printf("%s%s ", GRAY, BIG_N[i]); // N
        printf("%s%s ", GRAY, BIG_E[i]); // E
        printf("%s%s ", GRAY, BIG_M[i]); // M
        printf("%s%s ", GRAY, BIG_Y[i]); // Y

        printf("%s", RESET);
    }
}

void WindowRenderer::render_timer(int totalSec)
{
    int min = (totalSec / 60);
    int sec = totalSec % 60;

    set_cursor(middle_x + 4, 4);

    cout << BOLD;
    // 두 자리(setw(2))를 잡고, 빈 곳은 '0'으로 채움
    cout << setfill('0') << setw(2) << min % 100 << ":" << setw(2) << sec;
    cout << RESET;
}

void WindowRenderer::render_next_block(const int* tetrominoArray)
{
    for (size_t i = 0; i < 3; ++i) {
        int wr = middle_x + 3, wc = 11;

        Tetromino m1;
        Tetromino m2;
        Tetromino m3;
        m1.init_mino(tetrominoArray[0]);
        m2.init_mino(tetrominoArray[1]);
        m3.init_mino(tetrominoArray[2]);

        // next mino
        render_mino_pattern({wr, wc}, m1);

        render_mino_pattern({wr, wc + 5}, m2);

        render_mino_pattern({wr, wc + 10}, m3);
    }
}

void WindowRenderer::render_hold(const Tetromino& tetromino)
{
    render_mino_pattern({7, 11}, tetromino);
}

void WindowRenderer::draw_ui_box(const string& title, int x, int y, int w, int h, const char* color)
{
    set_cursor(x, y);
    cout << color << "┌" << string(w * 2, ' ') << "┐" << RESET;
    for (int i = 1; i <= h; ++i) {
        set_cursor(x, y + i);
        cout << color << "│" << string(w * 2, ' ') << "│" << RESET;
    }
    set_cursor(x, y + h + 1);
    cout << color << "└" << string(w * 2, ' ') << "┘" << RESET;
    if (!title.empty()) {
        set_cursor(x + (w * 2 - title.length()) / 2, y);
        cout << color << BOLD << "[" << title << "]" << RESET;
    }
}
void WindowRenderer::render_mino_pattern(Pos pos, const Tetromino& tetromino)
{
    if (tetromino.get_mino_type() < 0 || tetromino.get_mino_type() > 6) return;

    string color = get_block_color(tetromino);
    string bg = get_color(ColorKey::BACKGROUND, true);

    for (int i = 0; i < 4; i++) {
        string line;
        for (int j = 0; j < 4; j++) {
            if (tetromino.get_shape()[i][j] != 0) {
                line += color + bg + "██";
            }
            else {
                line += bg + "  ";
            }
        }

        set_cursor(pos.x, pos.y + i);
        printf("%s", line.c_str());
    }
}

void WindowRenderer::render_mino_pattern(int x, int y, const Mino& shape, const char* color)
{
    for (int i = 0; i < 4; i++) {
        set_cursor(x, y + i);
        for (int j = 0; j < 4; j++) {
            if (shape[i][j] != 0) { // 배열 값이 0이 아니면 출력
                cout << color << "██" << RESET;
            }
            else {
                cout << GRAY << "  " << RESET;
            }
        }
    }
}

void WindowRenderer::render_background()
{
    fflush(stdout);
    draw_logo(logo_x, 1);
    draw_ui_box("HOLD", 4, 9, 6, 4, GREEN);
    draw_ui_box("NEXT", middle_x, 9, 6, 15, PURPLE);
    draw_ui_box("#SCORE", 4, 16, 6, 3, CYAN);
    draw_ui_box("LV", 4, 21, 6, 3, CYAN);
    draw_ui_box("TIME", middle_x, 2, 6, 3, RESET);

    set_cursor(board_x, 7);
    cout << BOLD << "┏" << "━━━━━━━━━━━━━━━━━━━━━━" << "┓" << RESET;
    set_cursor(board_x, 28);
    cout << BOLD << "┗" << "━━━━━━━━━━━━━━━━━━━━━━" << "┛" << RESET;
}
void WindowRenderer::draw_hold(const Mino& hold_shape)
{
    render_mino_pattern(16, 3, hold_shape, YELLOW);
}
void WindowRenderer::render_score(int score)
{
    set_cursor(8, 18);
    printf("%s%06d%s", BOLD, score, RESET);
}
void WindowRenderer::render_level(int level)
{
    set_cursor(10, 23);
    printf("%s%02d%s", BOLD, level, RESET);
}
WindowRenderer::~WindowRenderer() {}

void WindowRenderer::hide_cursor()
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE; // FALSE로 설정하여 숨김
    SetConsoleCursorInfo(console_handle, &info);
}

void WindowRenderer::show_cursor()
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE; // 다시 보이게 설정
    SetConsoleCursorInfo(console_handle, &info);
}
void WindowRenderer::render_game_over()
{
    set_cursor(33, 17);
    printf("%s%s%s", RED, "GAMEOVER", RESET);
}
void WindowRenderer::render_board(const Board& board, const Tetromino& tetromino)
{
    int start_x = board_x;
    int start_y = 7;

    // 1. 보드 데이터 가져오기 (int[22][10] 배열이라고 가정)
    auto game_board = board.get_board();

    // 2. 현재 떨어지는 미노 정보 가져오기
    auto [pos_r, pos_c] = tetromino.get_pos(); // 현재 위치 (Top-Left)
    const Mino& shape = tetromino.get_shape(); // 현재 회전 상태의 4x4 배열 (int[4][4])

    // 행 루프 (2~21)
    for (int r = 2; r < 22; ++r) {
        set_cursor(start_x, start_y + (r - 1));
        cout << BOLD << "┃ " << RESET;

        // 열 루프 (0~9)
        for (int c = 0; c < 10; ++c) {
            bool is_falling_block = false;
            int block_type = 0;

            // --- [핵심 변경] 비트 연산 제거 -> 좌표 비교 ---

            // 1. 현재 좌표(r, c)가 떨어지는 미노의 4x4 영역 안에 있는지 확인
            bool inside_box = (r >= pos_r && r < pos_r + 4) && (c >= pos_c && c < pos_c + 4);

            if (inside_box) {
                // 4x4 배열 내부에서의 상대 좌표 계산 (0~3)
                int local_r = r - pos_r;
                int local_c = c - pos_c;

                // 해당 위치에 블록이 채워져 있는지 확인 (0이 아니면 블록임)
                if (shape[local_r][local_c] != 0) {
                    is_falling_block = true;
                    // 미노 자체의 타입을 가져오거나, shape의 값을 사용
                    block_type = tetromino.get_mino_type(); // 또는 shape[local_r][local_c]
                }
            }

            // --- 그리기 로직 ---

            if (is_falling_block) {
                // 떨어지는 블록 그리기
                print_s("██", get_color_key(tetromino));
            }
            else if (game_board[r][c] < 8 && game_board[r][c] > -1) {
                // 바닥에 쌓인 블록 그리기
                print_s("██", get_color_key(game_board[r][c]));
            }
            else {
                // 빈 공간
                cout << GRAY << ". " << RESET;
            }
        }

        cout << BOLD << " ┃" << RESET;
    }
}
void WindowRenderer::render_other_board(Packet& pkt)
{
    int start_x = 80; // wc (원래 코드의 50)
    int start_y = 7;  // wr (원래 코드의 7)

    // 2. 패킷에서 데이터 추출
    int pos_r = pkt.r - 2;
    int pos_c = pkt.c;
    int mino_type = pkt.type;
    int rotation = pkt.rotation;

    // 미노 모양 가져오기 (TETROMINO 배열이 전역이거나 클래스 멤버여야 함)
    const Mino& shape = TETROMINO[mino_type][rotation];
    string mino_color = get_block_color(mino_type); // 기존에 만든 색상 함수 활용

    set_cursor(start_x, start_y);
    cout << BOLD << "┏" << "━━━━━━━━━━━━━━━━━━━━━━" << "┓" << RESET;

    // 행 루프 (2~21)
    for (int r = 0; r < 20; ++r) {
        set_cursor(start_x, start_y + (r + 1));
        cout << BOLD << "┃ " << RESET;

        // 열 루프 (0~9)
        for (int c = 0; c < 10; ++c) {
            bool is_falling_block = false;

            // --- [핵심 변경] 비트 연산 제거 -> 좌표 비교 ---

            // 1. 현재 좌표(r, c)가 떨어지는 미노의 4x4 영역 안에 있는지 확인
            bool inside_box = (r >= pos_r && r < pos_r + 4) && (c >= pos_c && c < pos_c + 4);

            if (inside_box) {
                // 4x4 배열 내부에서의 상대 좌표 계산 (0~3)
                int local_r = r - pos_r;
                int local_c = c - pos_c;

                // 해당 위치에 블록이 채워져 있는지 확인 (0이 아니면 블록임)
                if (shape[local_r][local_c] != 0) {
                    is_falling_block = true;
                    // 미노 자체의 타입을 가져오거나, shape의 값을 사용
                }
            }

            // --- 그리기 로직 ---

            if (is_falling_block) {
                // 떨어지는 블록 그리기
                cout << get_block_color(pkt.type) << "██" << RESET;
            }
            else if (pkt.board[r][c] < 8 && pkt.board[r][c] > -1) {
                // 바닥에 쌓인 블록 그리기
                cout << get_block_color(pkt.board[r][c]) << "██" << RESET;
            }
            else {
                // 빈 공간
                cout << GRAY << ". " << RESET;
            }
        }

        cout << BOLD << " ┃" << RESET;
    }

    set_cursor(start_x, start_y + 21);
    cout << BOLD << "┗" << "━━━━━━━━━━━━━━━━━━━━━━" << "┛" << RESET;
}

void WindowRenderer::render_ip_recv()
{
    set_cursor(0, 0); // 적절한 위치로 수정 필요
    cout << "대전 상대의 IP를 입력하세요: ";
}

void WindowRenderer::render_char(char c) { cout << c; }

void WindowRenderer::render_clear()
{
    printf("\033[H");
    printf("%s", Theme::getInstance().color(ColorKey::BACKGROUND, true).c_str());
    printf("\033[2J");

    fflush(stdout);
}

void WindowRenderer::setting_arrow(int point_cur_setting)
{ // 현재 수정중인 메뉴 화살표 출력 및 다른 설정 화살표 지우기
    int x = 60;
    int setting_y_list[2] = {10, 15};
    for (int i = 0; i < 2; i++) {
        set_cursor(x, setting_y_list[i]);
        if (point_cur_setting == i)
            cout << "<-";
        else
            cout << "  ";
    }
}

void WindowRenderer::print_s(const char* const s, ColorKey key)
{
    printf("%s%s%s%s", get_color(key).c_str(), get_color(ColorKey::BACKGROUND, true).c_str(), s,
           get_color(ColorKey::BACKGROUND).c_str());
}

void WindowRenderer::print_s(string& s, ColorKey key) { print_s(s.c_str(), key); }

void WindowRenderer::print_big_char(Pos pos, char c, ColorKey key)
{
    for (int i = 0; i < 5; ++i) {
        set_cursor(pos.x, pos.y + i);
        print_s(BIG_FONT[c - 'A'][i], key);
    }
}

void WindowRenderer::print_big_char(Pos pos, char c) { print_big_char(pos, c, get_random_color()); }

void WindowRenderer::print_big_string(Pos pos, string& str, ColorKey key)
{
    for (int j = 0; j < str.size(); ++j) {
        char c = str[j];
        Pos next{pos.x + j * 6, pos.y};
        print_big_char(next, c);
    }
}

void WindowRenderer::print_big_string(Pos pos, string& str)
{
    for (int j = 0; j < str.size(); ++j) {
        ColorKey random_color = get_random_color();
        char c = str[j];
        for (int i = 0; i < 5; ++i) {
            set_cursor(pos.x + j * 6, pos.y + i);
            print_s(BIG_FONT[c - 'A'][i], random_color);
        }
    }
}

void WindowRenderer::print_big_string(Pos pos, const char* str)
{
    string s;
    s.assign(str);
    print_big_string(pos, s);
}

void WindowRenderer::print_small_string(Pos pos, string& str, ColorKey key)
{
    set_cursor(pos.x, pos.y);
    print_s(str, key);
}

void WindowRenderer::print_small_string(Pos pos, string& str)
{
    print_small_string(pos, str, get_random_color());
}

void WindowRenderer::print_small_string(Pos pos, const char* str)
{
    string s;
    s.assign(str);
    print_small_string(pos, s);
}

ColorKey WindowRenderer::get_random_color() { return static_cast<ColorKey>(dist(gen)); }
