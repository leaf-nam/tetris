#include "terminal_renderer.hpp"

#include <string>
#include <wtypes.h>

bool is_single = false;
int logo_x = (is_single) ? 20 : 2;
int board_x = (is_single) ? 35 : 25;
int middle_x = (is_single) ? 80 : 58;

namespace Color {
const char* const RESET = "\x1b[0m";
const char* const RED = "\x1b[31m";
const char* const GREEN = "\x1b[32m";
const char* const YELLOW = "\x1b[33m";
const char* const CYAN = "\x1b[36m";
const char* const PURPLE = "\x1b[35m";
const char* const GRAY = "\x1b[90m";
const char* const BOLD = "\x1b[1m";
} // namespace Color

const char* const TT[] = {"█████", "  █  ", "  █  ", "  █  ", "  █  "};
const char* const EE[] = {"█████", "█    ", "███  ", "█    ", "█████"};
const char* const RR[] = {"████ ", "█   █", "████ ", "█  █ ", "█   █"};
const char* const II[] = {"███", " █ ", " █ ", " █ ", "███"};
const char* const SS[] = {" ████", "█    ", " ███ ", "    █", "████ "};
const char* const NN[] = {"██  █", "█ █ █", "█  ██", "█   █", "█   █"};
const char* const MM[] = {"█   █", "██ ██", "█ █ █", "█ █ █", "█   █"};
const char* const YY[] = {"█   █", "█   █", " █ █ ", "  █  ", "  █  "};

using namespace std;

const char* get_block_color(int type)
{
    switch (type) {
    case 0: // I 미노 (Cyan - 하늘색)
        return "\x1b[96m";
    case 1: // O 미노 (Yellow - 노란색)
        return "\x1b[93m";
    case 2: // Z 미노 (Red - 빨간색)
        return "\x1b[91m";
    case 3: // S 미노 (Green - 초록색)
        return "\x1b[92m";
    case 4: // J 미노 (Blue - 파란색)
        return "\x1b[94m";
    case 5:                // L 미노 (Orange -> 터미널은 보통 흰색 or 밝은 노랑으로 대체)
        return "\x1b[37m"; // 흰색(White) 사용 (노란색 O와 구분하기 위해)
    case 6:                // T 미노 (Purple - 보라색)
        return "\x1b[95m";
    case 7: // 방해 블록 (Gray - 회색)
        return "\x1b[90m";
    default: // 예외 (Reset)
        return "\x1b[0m";
    }
}

void TerminalRenderer::set_cursor(int x, int y) { printf("\033[%d;%dH", y + 1, x + 1); }

void TerminalRenderer::clear()
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
    printf("\e[?25l");
}

void TerminalRenderer::draw_logo()
{
    // 각 글자를 배열로 분리
    hide_cursor();

    int x = logo_x; // 시작 X 좌표
    int y = 1;      // 시작 Y 좌표

    for (int i = 0; i < 5; i++) {
        //
        set_cursor(x, y + i);
        // 순서대로 색상과 함께 출력
        printf("%s%s ", Color::RED, TT[i]);    // T
        printf("%s%s ", Color::YELLOW, EE[i]); // E
        printf("%s%s ", Color::GREEN, TT[i]);  // T
        printf("%s%s ", Color::CYAN, RR[i]);   // R
        printf("%s%s ", Color::PURPLE, II[i]); // I
        printf("%s%s ", Color::RED, SS[i]);    // S
        printf("%s%s ", Color::YELLOW, SS[i]); // S
        printf("%s%s ", Color::GREEN, EE[i]);  // E
        printf("%s%s", Color::CYAN, NN[i]);    // N
        printf("%s", Color::RESET);
    }
    if (!is_single) {
        for (int i = 0; i < 5; i++) {
            set_cursor(78, y + i);
            // 순서대로 색상과 함께 출력

            printf("%s%s ", Color::GRAY, EE[i]); // E
            printf("%s%s ", Color::GRAY, NN[i]); // N
            printf("%s%s ", Color::GRAY, EE[i]); // E
            printf("%s%s ", Color::GRAY, MM[i]); // M
            printf("%s%s ", Color::GRAY, YY[i]); // Y

            printf("%s", Color::RESET);
        }
    }
    // v1 출력 (N 글자 아래쪽 끝에 배치)
    set_cursor(x + 52, y + 4);
    printf("%s%s%s", Color::GRAY, "v1", Color::RESET);
}

void TerminalRenderer::render_timer(int totalSec)
{
    int min = (totalSec / 60);
    int sec = totalSec % 60;

    set_cursor(middle_x + 4, 4);

    cout << Color::BOLD;
    // 두 자리(setw(2))를 잡고, 빈 곳은 '0'으로 채움
    cout << setfill('0') << setw(2) << min % 100 << ":" << setw(2) << sec;
    cout << Color::RESET;
}

void TerminalRenderer::render_next_block(const int* tetrominoArray)
{
    for (size_t i = 0; i < 3; ++i) {
        int wr = middle_x + 3, wc = 11;
        const Mino& m1 = TETROMINO[tetrominoArray[0]][0];
        const Mino& m2 = TETROMINO[tetrominoArray[1]][0];
        const Mino& m3 = TETROMINO[tetrominoArray[2]][0];

        // next mino
        render_mino_pattern(wr, wc, m1, get_block_color(tetrominoArray[0]));

        render_mino_pattern(wr, wc + 5, m2, get_block_color(tetrominoArray[1]));

        render_mino_pattern(wr, wc + 10, m3, get_block_color(tetrominoArray[2]));
    }
}

void TerminalRenderer::render_hold(const Tetromino& tetromino)
{
    if (tetromino.get_mino_type() < 0 || tetromino.get_mino_type() > 6) return;
    render_mino_pattern(7, 11, tetromino.get_shape(), get_block_color(tetromino.get_mino_type()));
}

void TerminalRenderer::draw_ui_box(const string& title, int x, int y, int w, int h,
                                   const char* color)
{
    set_cursor(x, y);
    cout << color << "┌" << string(w * 2, ' ') << "┐" << Color::RESET;
    for (int i = 1; i <= h; ++i) {
        set_cursor(x, y + i);
        cout << color << "│" << string(w * 2, ' ') << "│" << Color::RESET;
    }
    set_cursor(x, y + h + 1);
    cout << color << "└" << string(w * 2, ' ') << "┘" << Color::RESET;
    if (!title.empty()) {
        set_cursor(x + (w * 2 - title.length()) / 2, y);
        cout << color << Color::BOLD << "[" << title << "]" << Color::RESET;
    }
}
void TerminalRenderer::render_mino_pattern(int x, int y, const Mino& shape, const char* color)
{
    for (int i = 0; i < 4; i++) {
        set_cursor(x, y + i);
        for (int j = 0; j < 4; j++) {
            if (shape[i][j] != 0) { // 배열 값이 0이 아니면 출력
                cout << color << "██" << Color::RESET;
            }
            else {
                cout << Color::GRAY << "  " << Color::RESET;
            }
        }
    }
}

void TerminalRenderer::render_background()
{
    fflush(stdout);
    draw_logo();
    draw_ui_box("HOLD", 4, 9, 6, 4, Color::GREEN);
    draw_ui_box("NEXT", middle_x, 9, 6, 15, Color::PURPLE);
    draw_ui_box("#SCORE", 4, 16, 6, 3, Color::CYAN);
    draw_ui_box("LV", 4, 21, 6, 3, Color::CYAN);
    draw_ui_box("TIME", middle_x, 2, 6, 3, Color::RESET);
}
void TerminalRenderer::draw_hold(const Mino& hold_shape)
{
    render_mino_pattern(16, 3, hold_shape, Color::YELLOW);
}
void TerminalRenderer::render_score(int score)
{
    set_cursor(8, 18);
    printf("%s%06d%s", Color::BOLD, score, Color::RESET);
}
void TerminalRenderer::render_level(int level)
{
    set_cursor(10, 23);
    printf("%s%02d%s", Color::BOLD, level, Color::RESET);
}
TerminalRenderer::~TerminalRenderer() {}

void TerminalRenderer::hide_cursor()
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE; // FALSE로 설정하여 숨김
    SetConsoleCursorInfo(console_handle, &info);
}

void TerminalRenderer::show_cursor()
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE; // 다시 보이게 설정
    SetConsoleCursorInfo(console_handle, &info);
}
void TerminalRenderer::render_game_over()
{
    set_cursor(43, 16);
    printf("%s%s%s", Color::RED, "GAMEOVER", Color::RESET);
}
void TerminalRenderer::render_board(const Board& board, const Tetromino& tetromino)
{
    int start_x = board_x;
    int start_y = 7;

    // 1. 보드 데이터 가져오기 (int[22][10] 배열이라고 가정)
    auto game_board = board.get_board();

    // 2. 현재 떨어지는 미노 정보 가져오기
    auto [pos_r, pos_c] = tetromino.get_pos(); // 현재 위치 (Top-Left)
    const Mino& shape = tetromino.get_shape(); // 현재 회전 상태의 4x4 배열 (int[4][4])

    set_cursor(start_x, start_y);
    cout << Color::BOLD << "┏" << "━━━━━━━━━━━━━━━━━━━━━━" << "┓" << Color::RESET;

    // 행 루프 (2~21)
    for (int r = 2; r < 22; ++r) {
        set_cursor(start_x, start_y + (r - 1));
        cout << Color::BOLD << "┃ " << Color::RESET;

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
                cout << get_block_color(block_type) << "██" << Color::RESET;
            }
            else if (game_board[r][c] < 8 && game_board[r][c] > -1) {
                // 바닥에 쌓인 블록 그리기
                cout << get_block_color(game_board[r][c]) << "██" << Color::RESET;
            }
            else {
                // 빈 공간
                cout << Color::GRAY << ". " << Color::RESET;
            }
        }

        cout << Color::BOLD << " ┃" << Color::RESET;
    }

    set_cursor(start_x, start_y + 21);
    cout << Color::BOLD << "┗" << "━━━━━━━━━━━━━━━━━━━━━━" << "┛" << Color::RESET;
}
void TerminalRenderer::render_other_board(Packet& pkt)
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
    cout << Color::BOLD << "┏" << "━━━━━━━━━━━━━━━━━━━━━━" << "┓" << Color::RESET;

    // 행 루프 (2~21)
    for (int r = 0; r < 20; ++r) {
        set_cursor(start_x, start_y + (r + 1));
        cout << Color::BOLD << "┃ " << Color::RESET;

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
                cout << get_block_color(pkt.type) << "██" << Color::RESET;
            }
            else if (pkt.board[r][c] < 8 && pkt.board[r][c] > -1) {
                // 바닥에 쌓인 블록 그리기
                cout << get_block_color(pkt.board[r][c]) << "██" << Color::RESET;
            }
            else {
                // 빈 공간
                cout << Color::GRAY << ". " << Color::RESET;
            }
        }

        cout << Color::BOLD << " ┃" << Color::RESET;
    }

    set_cursor(start_x, start_y + 21);
    cout << Color::BOLD << "┗" << "━━━━━━━━━━━━━━━━━━━━━━" << "┛" << Color::RESET;
}

void TerminalRenderer::render_ip_recv()
{
    set_cursor(0, 0); // 적절한 위치로 수정 필요
    cout << "대전 상대의 IP를 입력하세요: ";
}

void TerminalRenderer::render_char(char c) { cout << c; }

void TerminalRenderer::render_clear() { system("cls"); }
