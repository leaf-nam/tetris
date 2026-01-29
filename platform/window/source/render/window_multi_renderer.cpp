#include "render/window_multi_renderer.hpp"

using namespace std;

const int BOARD_START_Y = 7;
const int MY_TOTAL_WIDTH = 45;
const int MY_TOTAL_HEIGHT = 30;
const int ENEMY_BOARD_WIDTH = 20;
const int ENEMY_BOARD_HEIGHT = 30;
const int BOARD_WIDTH = 22;
const int BOARD_HEIGHT = 21;

const int BOX_WIDTH = 10;
const int BOX_HEIGHT = 6;

const int MARGIN = 1;
const int GAP_X = 3;

const int HOLD_Y = BOARD_START_Y;
const int SCORE_Y = MARGIN * 2 + BOARD_START_Y + BOX_HEIGHT;
const int LV_Y = MARGIN * 4 + BOARD_START_Y + BOX_HEIGHT * 2;
const int TIMER_Y = BOARD_START_Y;
const int NEXT_Y = BOARD_START_Y + BOX_HEIGHT / 2 + MARGIN;

const int LEFT_BOX_X = MARGIN;
const int RIGHT_BOX_X = BOX_WIDTH + BOARD_WIDTH + MARGIN * 2;
const int MY_BOARD_X = MARGIN * 3 + BOX_WIDTH;

WindowMultiRenderer::WindowMultiRenderer(Setting* a1, ConsoleRenderer a2, ColorPicker a3,
                                         TextRenderer a4, BoxRenderer a5, BlockRenderer a6,
                                         ShadowMaker a7)
    : setting(a1), console_renderer(a2), color_picker(a3), text_renderer(a4), box_renderer(a5),
      block_renderer(a6), shadow_maker(a7)
{
}

void WindowMultiRenderer::render_clear() { console_renderer.clear(); }

void WindowMultiRenderer::render_background()
{
    fflush(stdout);

    // logo
    text_renderer.print_big_string({MARGIN, MARGIN}, "MYBOARD");

    // left box
    box_renderer.draw_box({LEFT_BOX_X, HOLD_Y}, BOX_WIDTH, BOX_HEIGHT, "[HOLD]", Color::GREEN,
                          Color::PANEL);
    box_renderer.draw_box({LEFT_BOX_X, SCORE_Y}, BOX_WIDTH, BOX_HEIGHT, "[SCORE]", Color::CYAN,
                          Color::PANEL);
    box_renderer.draw_box({LEFT_BOX_X, LV_Y}, BOX_WIDTH, BOX_HEIGHT, "[LV]", Color::CYAN,
                          Color::PANEL);

    // right box
    box_renderer.draw_box({RIGHT_BOX_X, TIMER_Y}, BOX_WIDTH, BOX_HEIGHT / 2, "[TIME]",
                          Color::COMMENT, Color::PANEL);
    box_renderer.draw_box({RIGHT_BOX_X, NEXT_Y}, BOX_WIDTH, BOX_HEIGHT * 3, "[NEXT]", Color::PURPLE,
                          Color::PANEL);

    // my board
    box_renderer.draw_line({MY_BOARD_X, BOARD_START_Y}, BOARD_WIDTH, BOARD_HEIGHT,
                           Color::FOREGROUND);

    Color enemy1_color = Color::CYAN;
    Color enemy2_color = Color::ORANGE;
    Color enemy3_color = Color::PINK;

    int start_margin = MARGIN * 2;
    int text_margin = MARGIN * 4;
    int board_margin = MARGIN * 3;

    // enemy 1 board
    text_renderer.print_big_string({start_margin + text_margin + MY_TOTAL_WIDTH, MARGIN}, "E1",
                                   enemy1_color);
    box_renderer.draw_line({start_margin + MY_TOTAL_WIDTH, BOARD_START_Y}, BOARD_WIDTH,
                           BOARD_HEIGHT, enemy1_color);

    // enemy 2 board
    text_renderer.print_big_string(
        {start_margin + board_margin + text_margin + MY_TOTAL_WIDTH + ENEMY_BOARD_WIDTH, MARGIN},
        "E2", enemy2_color);
    box_renderer.draw_line(
        {start_margin + board_margin + MY_TOTAL_WIDTH + ENEMY_BOARD_WIDTH, BOARD_START_Y},
        BOARD_WIDTH, BOARD_HEIGHT, enemy2_color);

    // enemy 3 board
    text_renderer.print_big_string(
        {start_margin + board_margin * 2 + text_margin + MY_TOTAL_WIDTH + ENEMY_BOARD_WIDTH * 2,
         MARGIN},
        "E3", enemy3_color);
    box_renderer.draw_line(
        {start_margin + board_margin * 2 + MY_TOTAL_WIDTH + ENEMY_BOARD_WIDTH * 2, BOARD_START_Y},
        BOARD_WIDTH, BOARD_HEIGHT, enemy3_color);
}

void WindowMultiRenderer::render_hold(const Tetromino& tetromino)
{
    block_renderer.render_mino_pattern({LEFT_BOX_X + 2, HOLD_Y + 2}, tetromino, Color::PANEL);
}

void WindowMultiRenderer::render_score(int score)
{
    string score_str = to_string(score);
    console_renderer.set_cursor(LEFT_BOX_X + 8 - (int) score_str.size(), SCORE_Y + 3);
    console_renderer.print_s(to_string(score), Color::CYAN, Color::PANEL);
}

void WindowMultiRenderer::render_level(int level)
{
    string level_str = to_string(level);
    text_renderer.print_big_string({LEFT_BOX_X + 3, LV_Y + 1}, level_str, Color::CYAN,
                                   Color::PANEL);
}

void WindowMultiRenderer::render_timer(int totalSec)
{
    int min = (totalSec / 60);
    int sec = totalSec % 60;

    string sec_str =
        to_string(min / 10) + to_string(min % 10) + ":" + to_string(sec / 10) + to_string(sec % 10);

    console_renderer.set_cursor(RIGHT_BOX_X + 3, TIMER_Y + 1);
    console_renderer.print_s(sec_str, Color::COMMENT, Color::PANEL);
}

void WindowMultiRenderer::render_next_block(const int* tetrominoArray)
{
    int x = RIGHT_BOX_X + 2, y = NEXT_Y + 2, diff = BOX_HEIGHT;
    for (size_t i = 0; i < 3; ++i) {
        Tetromino m1;
        Tetromino m2;
        Tetromino m3;
        m1.init_mino(tetrominoArray[0]);
        m2.init_mino(tetrominoArray[1]);
        m3.init_mino(tetrominoArray[2]);

        // next mino
        block_renderer.render_mino_pattern({x, y}, m1, Color::PANEL);
        block_renderer.render_mino_pattern({x, y + diff}, m2, Color::PANEL);
        block_renderer.render_mino_pattern({x, y + diff * 2}, m3, Color::PANEL);
    }
}

void WindowMultiRenderer::render_board(const Board& board, const Tetromino& tetromino)
{
    auto game_board = board.get_board();
    auto [pos_r, pos_c] = tetromino.get_pos();
    const Mino& shape = tetromino.get_shape();

    vector<Pos> shadows;
    if (setting->shadow_on) shadows = shadow_maker.get_shadow_pos(board, tetromino);

    for (int r = 2; r < 22; ++r) {
        console_renderer.set_cursor(MY_BOARD_X, BOARD_START_Y + (r - 1));

        for (int c = 0; c < 10; ++c) {
            bool is_falling_block = false;
            int block_type = 0;

            bool inside_box = (r >= pos_r && r < pos_r + 4) && (c >= pos_c && c < pos_c + 4);

            if (inside_box) {
                if (shape[r - pos_r][c - pos_c] != 0) {
                    is_falling_block = true;
                }
            }

            // 떨어지는 블록 그리기
            if (is_falling_block) {
                console_renderer.print_s("██", color_picker.get_color_key(tetromino));
            }

            // 바닥에 쌓인 블록 그리기
            else if (game_board[r][c] < 8 && game_board[r][c] > -1) {
                console_renderer.print_s("██", color_picker.get_color_key(game_board[r][c]));
            }

            // 그림자 그리기
            else if (setting->shadow_on && shadow_maker.is_shadow(shadows, {c, r})) {
                console_renderer.print_s("██", Color::COMMENT);
            }

            // 빈 공간
            else {
                console_renderer.print_s("  ", Color::BACKGROUND);
            }
        }
    }
}

void WindowMultiRenderer::render_other_board(Packet& pkt)
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
    string mino_color = color_picker.get_block_color(mino_type); // 기존에 만든 색상 함수 활용

    /*
    console_renderer.set_cursor(start_x, start_y);
    cout << BOLD << "┏" << "━━━━━━━━━━━━━━━━━━━━━━" << "┓" << RESET;

    // 행 루프 (2~21)
    for (int r = 0; r < 20; ++r) {
        console_renderer.set_cursor(start_x, start_y + (r + 1));
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
                cout << color_picker.get_block_color(pkt.type) << "██" << RESET;
            }
            else if (pkt.board[r][c] < 8 && pkt.board[r][c] > -1) {
                // 바닥에 쌓인 블록 그리기
                cout << color_picker.get_block_color(pkt.board[r][c]) << "██" << RESET;
            }
            else {
                // 빈 공간
                cout << GRAY << ". " << RESET;
            }
        }

        cout << BOLD << " ┃" << RESET;
    }

    console_renderer.set_cursor(start_x, start_y + 21);
    cout << BOLD << "┗" << "━━━━━━━━━━━━━━━━━━━━━━" << "┛" << RESET;
    */
}

//void WindowMultiRenderer::render_ip_recv()
//{
//    console_renderer.set_cursor(0, 0); // 적절한 위치로 수정 필요
//    cout << "대전 상대의 IP를 입력하세요: ";
//}
//
//void WindowMultiRenderer::render_char(char c) { cout << c; }

WindowMultiRenderer::~WindowMultiRenderer() {}
