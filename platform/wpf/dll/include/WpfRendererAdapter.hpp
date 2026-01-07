#include "api/IRenderer.hpp"

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

typedef void(*BackgroundCallback)();
typedef void(*BoardCallback)(Board&, Tetromino&);
typedef void(*TimerCallback)(int);
typedef void(*NextBlockCallback)(Tetromino*);
typedef void(*HoldCallback)(Tetromino&);
typedef void(*ScoreCallback)(int);
typedef void(*LevelCallback)(int);

class WpfRendererAdapter : public IRenderer {
private:
	BackgroundCallback backgroundCallback;
	BoardCallback boardCallback;
	TimerCallback timerCallback;
	NextBlockCallback nextBlockCallback;
	HoldCallback holdCallback;
	ScoreCallback scoreCallback;
	LevelCallback levelCallback;
public:
	WpfRendererAdapter();
	void renderBackground() override;
	void renderBoard(const Board& board, const Tetromino& tetromino) override;
	void renderTimer(const int sec) override;
	void renderNextBlock(const Tetromino* tetrominoArray) override;
	void renderHold(const Tetromino& tetromino) override;
	void renderScore(const int score) override;
	void renderLevel(const int level) override;
	~WpfRendererAdapter() override;
};

struct Callbacks {
	BackgroundCallback backgroundCallback;
	BoardCallback boardCallback;
	TimerCallback timerCallback;
	NextBlockCallback nextBlockCallback;
	HoldCallback holdCallback;
	ScoreCallback scoreCallback;
	LevelCallback levelCallback;
};

extern "C" {
	DLL_EXPORT void RegisterCallbacks(Callbacks callbacks);
}
