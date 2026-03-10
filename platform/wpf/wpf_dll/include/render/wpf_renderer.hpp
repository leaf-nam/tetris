#ifndef __WPF_RENDERED_HPP__
#define __WPF_RENDERED_HPP__

#include "callback/callbacks.hpp"
#include "api/i_renderer.hpp"

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

class WpfRenderer : public IRenderer 
{
private:
	Callbacks* cbs;
public:
	WpfRenderer();

	void set_callback(Callbacks*);

	void renderBackground() override;
	void renderBoard(const Board&, const Tetromino&) override;
	void renderTimer(const int) override;
	void renderNextBlock(const int*) override;
	void renderHold(const Tetromino&) override;
	void renderScore(const int) override;
	void renderLevel(const int) override;

	~WpfRenderer() override;
};

#endif