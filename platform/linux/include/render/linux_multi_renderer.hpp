#ifndef __LINUX_MULTI_RENDERER_H__
#define __LINUX_MULTI_RENDERER_H__

#include "render/block_renderer.hpp"
#include "render/box_renderer.hpp"
#include "render/color.hpp"
#include "render/color_picker.hpp"
#include "render/input_window_renderer.hpp"
#include "render/pos.hpp"
#include "render/text_renderer.hpp"
#include "render/theme.hpp"
#include "setting.hpp"
#include "util/shadow_maker.hpp"
#include "linux_console_renderer.hpp"

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

class LinuxMultiRenderer : public IRenderer
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
    std::pair<int, int> other_render_loc_array[3];
    int other_render_index;

    std::pair<int, int> other_render_loc_get_or_set(std::string id);

  public:
    LinuxMultiRenderer(Setting*, IPlatformRenderer*, ColorPicker, TextRenderer, BoxRenderer, BlockRenderer, ShadowMaker);
    
    void render_background() override;

    void render_board(const Board& board, const Tetromino& tetromino) override;

    void render_timer(int sec) override;

    void render_next_block(const int* tetrominoArray) override;

    void render_hold(const Tetromino& tetromino) override;

    void render_score(int score) override;

    void render_level(int level) override;

    void render_other_board(Packet& pkt) override;

    void render_clear() override;

    void render_game_over() override;

    void render_other_game_over(Packet& pkt) override;

    void render_win() override;

    void render_other_win(Packet& pkt) override;
    
    virtual ~LinuxMultiRenderer() override;
};

#endif
