#include "render/window_renderer.hpp"

using namespace std;

bool is_single = false;
int logo_x = (is_single) ? 20 : 2;
int board_x = (is_single) ? 35 : 25;
int middle_x = (is_single) ? 80 : 58;

WindowRenderer::WindowRenderer(Setting* a1, ConsoleRenderer a2, ColorPicker a3, TextRenderer a4,
                               BoxRenderer a5, BlockRenderer a6, InputWindowRenderer a7,
                               ShadowMaker a8)
    : setting(a1), console_renderer(a2), color_picker(a3), text_renderer(a4), box_renderer(a5),
      block_renderer(a6), input_window_renderer(a7)
{
}

void WindowRenderer::render_timer(int totalSec)
{
    int min = (totalSec / 60);
    int sec = totalSec % 60;

    console_renderer.set_cursor(middle_x + 4, 4);

    cout << get_color(Color::CYAN);
    // 두 자리(setw(2))를 잡고, 빈 곳은 '0'으로 채움
    cout << setfill('0') << setw(2) << min % 100 << ":" << setw(2) << sec;
    cout << Theme::reset();
}

void WindowRenderer::render_clear() { console_renderer.clear(); }
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
        block_renderer.render_mino_pattern({wr, wc}, m1);
        block_renderer.render_mino_pattern({wr, wc + 5}, m2);
        block_renderer.render_mino_pattern({wr, wc + 10}, m3);
    }
}

void WindowRenderer::render_hold(const Tetromino& tetromino)
{
    block_renderer.render_mino_pattern({7, 11}, tetromino);
}

void WindowRenderer::render_background()
{
    fflush(stdout);
    text_renderer.draw_logo({logo_x, 1});
    box_renderer.draw_box({4, 9}, 6, 4, "HOLD", Color::GREEN);
    box_renderer.draw_box({middle_x, 9}, 6, 15, "NEXT", Color::PURPLE);
    box_renderer.draw_box({4, 16}, 6, 3, "#SCORE", Color::CYAN);
    box_renderer.draw_box({4, 21}, 6, 3, "LV", Color::CYAN);
    box_renderer.draw_box({middle_x, 2}, 6, 3, "TIME", Color::COMMENT);

    console_renderer.set_cursor(board_x, 7);
    console_renderer.print_s(" ██████████████████████ ", Color::FOREGROUND);

    console_renderer.set_cursor(board_x, 28);
    console_renderer.print_s(" ██████████████████████ ", Color::FOREGROUND);
}

void WindowRenderer::render_score(int score)
{
    console_renderer.set_cursor(8, 18);
    console_renderer.print_s("score", Color::CYAN);
}

void WindowRenderer::render_level(int level)
{
    console_renderer.set_cursor(10, 23);
    console_renderer.print_s("level", Color::CYAN);
}

void WindowRenderer::render_board(const Board& board, const Tetromino& tetromino)
{
    int start_x = board_x;
    int start_y = 7;
    auto game_board = board.get_board();
    auto [pos_r, pos_c] = tetromino.get_pos();
    const Mino& shape = tetromino.get_shape();

    vector<Pos> shadows;
    if (setting->shadow_on) shadows = shadow_maker.get_shadow_pos(board, tetromino);

    for (int r = 2; r < 22; ++r) {
        console_renderer.set_cursor(start_x, start_y + (r - 1));
        console_renderer.print_s(" █", Color::FOREGROUND);

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
            else if (setting->shadow_on && shadow_maker.isShadow(shadows, {c, r})) {
                console_renderer.print_s("██", Color::COMMENT);
            }

            // 빈 공간
            else {
                console_renderer.print_s("  ", Color::BACKGROUND);
            }
        }

        console_renderer.print_s("█ ", Color::FOREGROUND);
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

void WindowRenderer::render_ip_recv()
{
    console_renderer.set_cursor(0, 0); // 적절한 위치로 수정 필요
    cout << "대전 상대의 IP를 입력하세요: ";
}

void WindowRenderer::render_char(char c) { cout << c; }

WindowRenderer::~WindowRenderer() {}
