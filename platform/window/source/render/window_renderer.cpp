#include "render/window_renderer.hpp"

using namespace std;

const int LOGO_X = 20;
const int BOARD_X = 35;
const int BOARD_Y = 7;
const int LEFT_X = 4;
const int MIDDLE_X = 80;

WindowRenderer::WindowRenderer(Setting* a1, ConsoleRenderer a2, ColorPicker a3, TextRenderer a4,
                               BoxRenderer a5, BlockRenderer a6, ShadowMaker a7)
    : setting(a1), console_renderer(a2), color_picker(a3), text_renderer(a4), box_renderer(a5),
      block_renderer(a6), shadow_maker(a7)
{
}

WindowRenderer::WindowRenderer()
{ 
    other_render_loc_array[0] = std::make_pair(80, 7);
    other_render_loc_array[1] = std::make_pair(101, 7);
    other_render_loc_array[2] = std::make_pair(80, 19);
    other_render_loc_array[3] = std::make_pair(101, 19);

    other_render_index = 0;
}

std::pair<int, int> WindowRenderer::other_render_loc_get_or_set(std::string id)
{
    if (other_land_index_map.find(id) == other_land_index_map.end())
    {
        other_land_index_map[id] = other_render_index;
        return other_render_loc_array[other_render_index++];
    }
    else
        return other_render_loc_array[other_land_index_map[id]];
}

void WindowRenderer::render_timer(int totalSec)
{
    int min = (totalSec / 60);
    int sec = totalSec % 60;

    string sec_str =
        to_string(min / 10) + to_string(min % 10) + ":" + to_string(sec / 10) + to_string(sec % 10);

    console_renderer.set_cursor(MIDDLE_X + 2, 4);
    console_renderer.print_s(sec_str, Color::COMMENT);
}

void WindowRenderer::render_clear() { console_renderer.clear(); }
void WindowRenderer::render_next_block(const int* tetrominoArray)
{
    for (size_t i = 0; i < 3; ++i) {
        int x = MIDDLE_X + 1, y = 11;

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
    block_renderer.render_mino_pattern({5, 11}, tetromino, Color::PANEL);
}

void WindowRenderer::render_background()
{
    fflush(stdout);
    text_renderer.draw_logo({LOGO_X, 1});
    box_renderer.draw_box({LEFT_X, 9}, 9, 6, "[HOLD]", Color::GREEN, Color::PANEL);
    box_renderer.draw_box({MIDDLE_X, 9}, 9, 17, "[NEXT]", Color::PURPLE, Color::PANEL);
    box_renderer.draw_box({LEFT_X, 16}, 9, 4, "[SCORE]", Color::CYAN, Color::PANEL);
    box_renderer.draw_box({LEFT_X, 21}, 9, 4, "[LV]", Color::CYAN, Color::PANEL);
    box_renderer.draw_box({MIDDLE_X, 2}, 9, 4, "[TIME]", Color::COMMENT, Color::PANEL);
    box_renderer.draw_line({BOARD_X, BOARD_Y}, 22, 21, Color::FOREGROUND);
}

void WindowRenderer::render_score(int score)
{
    string score_str = to_string(score);
    console_renderer.set_cursor(12 - score_str.size(), 18);
    console_renderer.print_s(to_string(score), Color::CYAN);
}

void WindowRenderer::render_level(int level)
{
    string level_str = to_string(level);
    console_renderer.set_cursor(12 - level_str.size(), 23);
    console_renderer.print_s(level_str, Color::CYAN);
}

void WindowRenderer::render_board(const Board& board, const Tetromino& tetromino)
{
    auto game_board = board.get_board();
    auto [pos_r, pos_c] = tetromino.get_pos();
    const Mino& shape = tetromino.get_shape();

    vector<Pos> shadows;
    if (setting->shadow_on) shadows = shadow_maker.get_shadow_pos(board, tetromino);

    for (int r = 2; r < 22; ++r) {
        console_renderer.set_cursor(BOARD_X, BOARD_Y + (r - 1));

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

void WindowRenderer::render_other_board(Packet& pkt)
{
    // not used
}

void WindowRenderer::render_ip_recv()
{
    console_renderer.set_cursor(0, 0); // 적절한 위치로 수정 필요
    cout << "대전 상대의 IP를 입력하세요: ";
}

void WindowRenderer::render_char(char c) { cout << c; }

WindowRenderer::~WindowRenderer() {}
