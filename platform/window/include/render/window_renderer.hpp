#ifndef window_RENDERER_H
#define window_RENDERER_H

#include "board/board.hpp"
#include "tetromino/tetromino.hpp"

#include <cstdint>
#include <cstdio>
#include <i_renderer.hpp>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// --- 상수 정의 ---
static const uint16_t LEFT_EDGE = 1u << 12;
static const uint16_t RIGHT_EDGE = 1u << 3;

// --- 색상 네임스페이스 ---
namespace Color {
extern const char* const RESET;
extern const char* const RED;
extern const char* const GREEN;
extern const char* const YELLOW;
extern const char* const CYAN;
extern const char* const PURPLE;
extern const char* const GRAY;
extern const char* const BOLD;
} // namespace Color

class WindowRenderer : public IRenderer
{
  public:
    /**
     * @brief 게임 로직과 무관한 배경 렌더링
     */
    void render_background() override;

    /**
     * @brief 게임판 렌더링
     * @param 보드 현상태 + 현재 테트로미노 렌더링
     */
    void render_board(const Board& board, const Tetromino& tetromino) override;

    /**
     * @brief 타이머에 현재 시간 렌더링
     * @param 현재시간(초)
     */
    void render_timer(int sec) override;

    /**
     * @brief 다음 블럭 3개 렌더링
     * @param 다음 블럭 3개를 가진 배열 포인터(순서 중요)
     */
    void render_next_block(const int* tetrominoArray) override;

    /**
     * @brief 홀드할 블럭 렌더링
     * @param 홀드할 블럭
     */
    void render_hold(const Tetromino& tetromino) override;

    /**
     * @brief 점수판 렌더링
     * @param 현재 점수
     */
    void render_score(int score) override;

    /**
     * @brief 레벨 렌더링
     * @param 현재 레벨
     */
    void render_level(int level) override;

    /**
     * @brief 소멸자
     */
    virtual ~WindowRenderer() override;

    void render_other_board(Packet& pkt) override;
    void render_ip_recv() override;

    void render_char(char c) override;

    void render_clear() override;

    void render_game_over();

    void render_menu(int menu_num);
    void render_settings();
    void render_solo_background();
    void draw_logo(int x, int y);

  private:
    void clear();
    // display
    void draw_enemy_title(int x, int y);
    void draw_ui_box(const string& title, int x, int y, int w, int h, const char* color);

    // draw
    void render_mino_pattern(int x, int y, const Mino& shape, const char* color);
    void draw_hold(const Mino& hold_shape);
    // hide, show cursor
    void show_cursor();
    void hide_cursor();

    void setting_arrow(int point_cur_setting);

    void set_cursor(int x, int y);
};

#endif