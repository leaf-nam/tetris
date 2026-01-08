#ifndef __WPF_RENDERED_HPP__
#define __WPF_RENDERED_HPP__

#include "callbacks.hpp"
#include "api/i_renderer.hpp"

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

class WpfRenderer : public IRenderer 
{
private:
	Callbacks cbs;
public:
	WpfRenderer();

	void set_callback(const Callbacks);

	void render_background() override;
	void render_board(const Board&, const Tetromino&) override;
	void render_timer(const int) override;
	void render_next_block(const Tetromino*) override;
	void render_hold(const Tetromino&) override;
	void render_score(const int) override;
	void render_level(const int) override;

	~WpfRenderer() override;
};

#endif