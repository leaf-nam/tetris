#include <emscripten/emscripten.h>
#include "render/i_renderer.hpp"
#include "board/board.hpp"
#include "tetromino/tetromino.hpp"
#include "util/network_packet.hpp"

EM_JS(void, js_render_background, (), {
  Module.webRenderer?.render_background();
});

EM_JS(void, js_render_timer, (int sec), {
  Module.webRenderer?.render_timer(sec);
});

EM_JS(void, js_render_score, (int score), {
  Module.webRenderer?.render_score(score);
});

EM_JS(void, js_render_level, (int level), {
  Module.webRenderer?.render_level(level);
});

EM_JS(void, js_render_clear, (), {
  Module.webRenderer?.render_clear();
});

EM_JS(void, js_render_game_over, (), {
  Module.webRenderer?.render_game_over();
});


static int g_board_flat[BOARD_ROW * BOARD_COL];
static int g_active_shape[16];
static int g_meta[4]; 

extern "C" EMSCRIPTEN_KEEPALIVE void export_render_state(const Board* b)
{
  const BoardT& bb = b->get_board();
  int k = 0;
  for (int r = 0; r < BOARD_ROW; r++)
    for (int c = 0; c < BOARD_COL; c++)
      g_board_flat[k++] = bb[r][c];

  auto [ar, ac] = b->get_active_mino_pos();
  int rot = b->get_active_mino_rotation();
  int type = b->get_active_mino_type();
  g_meta[0] = ar; g_meta[1] = ac; g_meta[2] = rot; g_meta[3] = type;

  const Mino& m = b->get_active_mino().get_shape(rot);
  int idx = 0;
  for (int r = 0; r < MINO_SIZE; r++)
    for (int c = 0; c < MINO_SIZE; c++)
      g_active_shape[idx++] = m[r][c];
}

extern "C" EMSCRIPTEN_KEEPALIVE int* get_board_ptr() { return g_board_flat; }
extern "C" EMSCRIPTEN_KEEPALIVE int* get_active_shape_ptr() { return g_active_shape; }
extern "C" EMSCRIPTEN_KEEPALIVE int* get_meta_ptr() { return g_meta; }

EM_JS(void, js_render_board_from_heap, (), {
  Module.webRenderer?.render_board_from_heap();
});

class WebRenderer final : public IRenderer {
public:
  void render_background() override { js_render_background(); }

  void render_board(const Board& board, const Tetromino&) override {
    export_render_state(&board);
    js_render_board_from_heap();
  }

  void render_timer(int sec) override { js_render_timer(sec); }
  void render_next_block(const int* tetrominoArray) override {
    
    (void)tetrominoArray;
  }

  void render_hold(const Tetromino&) override {
  }

  void render_score(int score) override { js_render_score(score); }
  void render_other_board(Packet&) override {}
  void render_game_over() override { js_render_game_over(); }
  void render_other_game_over(Packet&) override {}
  void render_win() override {}
  void render_other_win(Packet&) override {}
  void render_level(int level) override { js_render_level(level); }
  void render_clear() override { js_render_clear(); }
};