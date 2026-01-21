#ifndef __WINDOW_RENDERER_H__
#define __WINDOW_RENDERER_H__

#include "board/board.hpp"
#include "render/color.hpp"
#include "render/pos.hpp"
#include "tetromino/tetromino.hpp"

#include <cstdint>
#include <cstdio>
#include <i_renderer.hpp>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

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

    void render_solo_background();

    void draw_logo(int x, int y);

  protected:
    void clear();
    // display
    void draw_enemy_title(int x, int y);
    void draw_ui_box(const std::string& title, int x, int y, int w, int h, const char* color);

    // draw
    void render_mino_pattern(int x, int y, const Mino& shape, const char* color);
    void draw_hold(const Mino& hold_shape);

    // hide, show cursor
    void show_cursor();
    void hide_cursor();

    void setting_arrow(int point_cur_setting);
    void set_cursor(int x, int y);

    void print_big_char(Pos, char, ColorKey);
    void print_big_char(Pos, char);
    void print_big_string(Pos, std::string, ColorKey);
    void print_big_string(Pos, std::string);
    void print_small_string(Pos, std::string, ColorKey);
    void print_small_string(Pos, std::string);

    ColorKey get_random_color();

    const char* get_block_color(int type);
};

namespace color {
extern const char* const RESET;
extern const char* const RED;
extern const char* const GREEN;
extern const char* const YELLOW;
extern const char* const CYAN;
extern const char* const PURPLE;
extern const char* const GRAY;
extern const char* const BOLD;
} // namespace color

extern const char* const BIG_T[];
extern const char* const BIG_E[];
extern const char* const BIG_R[];
extern const char* const BIG_I[];
extern const char* const BIG_S[];
extern const char* const BIG_N[];
extern const char* const BIG_M[];
extern const char* const BIG_Y[];
extern const char* const BIG_G[];
#endif
