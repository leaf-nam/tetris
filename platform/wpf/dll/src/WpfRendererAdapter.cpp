WpfRendererAdapter::WpfRendererAdapter() {
	
}

void WpfRendererAdapter::renderBackground() {
	backgroundCallback();
}

void WpfRendererAdapter::renderBoard(const Board& board, const Tetromino& tetromino) {
	boardCallback(board, tetromino);
}

void WpfRendererAdapter::renderTimer(const int sec) {
	timerCallback(sec);
}

void WpfRendererAdapter::renderNextBlock(const Tetromino* tetrominoArray) {
	nextBlockCallback(tetrominoArray);
}

void WpfRendererAdapter::renderHold(const Tetromino& tetromino) {
	holdCallback(tetrimino);
}

void WpfRendererAdapter::renderScore(const int score) {
	scoreCallback(score);
}

void WpfRendererAdapter::renderLevel(const int level) {
	levelCallback(level);
}

WpfRendererAdapter::~WpfRendererAdapter() {
}

extern "C" {
	void RegisterCallbacks(Callbacks cbs) {
		backgroundCallback = cbs.backgroundCallback;
		boardCallback = cbs.boardCallback;
		timerCallback = cbs.timerCallback;
		nextBlockCallback = cbs.nextBlockCallback;
		holdCallback = cbs.holdCallback;
		scoreCallback = cbs.scoreCallback;
		levelCallback = cbs.levelCallback;
	}
}