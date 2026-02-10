#ifndef __WINDOW_RENDERER_H__
#define __WINDOW_RENDERER_H__

#include "render/block_renderer.hpp"
#include "render/box_renderer.hpp"
#include "render/color.hpp"
#include "render/color_picker.hpp"
#include "render/input_window_renderer.hpp"
#include "render/pos.hpp"
#include "render/text_renderer.hpp"
#include "render/theme.hpp"
#include "render/window_console_renderer.hpp"
#include "setting.hpp"
#include "util/shadow_maker.hpp"

#include <board/board.hpp>
#include <cstdint>
#include <cstdio>
#include <i_renderer.hpp>
#include <iomanip>
#include <iostream>
#include <string>
#include <tetromino/tetromino.hpp>
#include <unordered_map>
#include <vector>

class WindowRenderer : public IRenderer
{
  private:
    Setting* setting;
    IPlatformRenderer* platform_renderer;
    ColorPicker color_picker;
    TextRenderer text_renderer;
    BoxRenderer box_renderer;
    BlockRenderer block_renderer;
    ShadowMaker shadow_maker;

    std::unordered_map<std::string, int> other_land_index_map;
    std::pair<int, int> other_render_loc_array[4];
    int other_render_index;

    std::pair<int, int> other_render_loc_get_or_set(std::string id);

  public:
    WindowRenderer(Setting*, IPlatformRenderer*, ColorPicker, TextRenderer, BoxRenderer,
                   BlockRenderer, ShadowMaker);
    /**
     * @brief 렌더링 로직 초기화
     */
    WindowRenderer();

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

    // void render_ip_recv() override;

    // void render_char(char c) override;

    void render_clear() override;

    // void render_game_over() override;

    // void render_other_game_over(Packet& pkt) override;

    // void render_win() override;

    // void render_other_win(Packet& pkt) override;
};
#endif
