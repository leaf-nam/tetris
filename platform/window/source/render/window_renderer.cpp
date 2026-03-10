#include "render/window_renderer.hpp"

using namespace std;

const int LOGO_X = 35;
const int BOARD_X = 55;
const int BOARD_Y = 7;
const int LEFT_X = 0;
const int MIDDLE_X = 80;

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

const int LEFT_BOX_X = MARGIN + LEFT_X;
const int RIGHT_BOX_X = BOX_WIDTH + BOARD_WIDTH + MARGIN * 2 + LEFT_X;
const int MY_BOARD_X = MARGIN * 3 + BOX_WIDTH + LEFT_X;

WindowRenderer::WindowRenderer(Setting* a1, IPlatformRenderer* a2, ColorPicker a3, TextRenderer a4,
                               BoxRenderer a5, BlockRenderer a6, ShadowMaker a7)
    : setting(a1), platform_renderer(a2), color_picker(a3), text_renderer(a4), box_renderer(a5),
      block_renderer(a6), shadow_maker(a7)
{
}

void WindowRenderer::render_timer(int totalSec)
{
    int min = (totalSec / 60);
    int sec = totalSec % 60;

    string sec_str =
        to_string(min / 10) + to_string(min % 10) + ":" + to_string(sec / 10) + to_string(sec % 10);

    platform_renderer->set_cursor(RIGHT_BOX_X + 2, 8);
    platform_renderer->print_s(sec_str, Color::COMMENT, Color::PANEL);
}

void WindowRenderer::render_clear() { platform_renderer->clear(); }
void WindowRenderer::render_next_block(const int* tetrominoArray)
{
    for (size_t i = 0; i < 3; ++i) {
        int x = RIGHT_BOX_X + 1, y = 15;

        Tetromino m1;
        Tetromino m2;
        Tetromino m3;
        m1.init_mino(tetrominoArray[0]);
        m2.init_mino(tetrominoArray[1]);
        m3.init_mino(tetrominoArray[2]);

        // next mino
        block_renderer.render_mino_pattern({x, y}, m1, Color::PANEL);
        block_renderer.render_mino_pattern({x, y + 5}, m2, Color::PANEL);
        block_renderer.render_mino_pattern({x, y + 10}, m3, Color::PANEL);
    }
}

void WindowRenderer::render_hold(const Tetromino& tetromino)
{
    block_renderer.render_mino_pattern({LEFT_BOX_X + 2, 9}, tetromino, Color::PANEL);
}

void WindowRenderer::render_background()
{
    fflush(stdout);
    text_renderer.draw_logo({LEFT_X, 1});
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
}

void WindowRenderer::render_score(int score)
{
    string score_str = to_string(score);
    platform_renderer->set_cursor(LEFT_BOX_X + 8 - (int) score_str.size(), SCORE_Y + 3);
    platform_renderer->print_s(to_string(score), Color::CYAN, Color::PANEL);
}

void WindowRenderer::render_level(int level)
{
    string level_str = to_string(level);
    text_renderer.print_big_string({LEFT_BOX_X + 3, LV_Y + 1}, level_str, Color::CYAN,
                                   Color::PANEL);
}

// void WindowRenderer::render_game_over()
//{
//     set_cursor(33, 17);
//     printf("%s%s%s", RED, "GAMEOVER", RESET);
// }
//
// void WindowRenderer::render_other_game_over(Packet& pkt)
//{
//     auto [start_x, start_y] = other_render_loc_get_or_set(std::string(pkt.id));
//     set_cursor(start_x + 8, start_y + 10);
//     printf("%s%s%s", RED, "GAMEOVER", RESET);
// }
//
// void WindowRenderer::render_win() {
//     set_cursor(35, 6);
//     printf("%s%s%s", GREEN, "WIN", RESET);
// }
//
// void WindowRenderer::render_other_win(Packet& pkt) {
//     auto [start_x, start_y] = other_render_loc_get_or_set(std::string(pkt.id));
//     set_cursor(start_x + 10, start_y - 1);
//     printf("%s%s%s", GREEN, "WIN", RESET);
// }

void WindowRenderer::render_board(const Board& board, const Tetromino& tetromino)
{
    auto game_board = board.get_board();
    auto [pos_r, pos_c] = tetromino.get_pos();
    const Mino& shape = tetromino.get_shape();

    vector<Pos> shadows;
    if (setting->shadow_on) shadows = shadow_maker.get_shadow_pos(board, tetromino);

    for (int r = 2; r < 22; ++r) {
        platform_renderer->set_cursor(MY_BOARD_X, BOARD_Y + (r - 1));

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
                platform_renderer->print_s("██", color_picker.get_color_key(tetromino));
            }

            // 바닥에 쌓인 블록 그리기
            else if (game_board[r][c] < 8 && game_board[r][c] > -1) {
                platform_renderer->print_s("██", color_picker.get_color_key(game_board[r][c]));
            }

            // 그림자 그리기
            else if (setting->shadow_on && shadow_maker.is_shadow(shadows, {c, r})) {
                platform_renderer->print_s("██", Color::COMMENT);
            }

            // 빈 공간
            else {
                platform_renderer->print_s("  ", Color::BACKGROUND);
            }
        }
    }
}

void WindowRenderer::render_other_board(Packet& pkt) {}

void WindowRenderer::render_game_over() {}

void WindowRenderer::render_other_game_over(Packet& pkt) {}

void WindowRenderer::render_win() {}

void WindowRenderer::render_other_win(Packet& pkt) {}

WindowRenderer::~WindowRenderer() {}
