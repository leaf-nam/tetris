#include "terminal_renderer.hpp"
#include <wtypes.h>
#include <string>


bool isSingle = true;
int logoX = (isSingle) ? 20 : 2;
int boardX = (isSingle) ? 35 : 25;
int middleX = (isSingle) ? 80 : 58;

namespace Color {
	const char* RESET = "\x1b[0m";
	const char* RED = "\x1b[31m";
	const char* GREEN = "\x1b[32m";
	const char* YELLOW = "\x1b[33m";
	const char* CYAN = "\x1b[36m";
	const char* PURPLE = "\x1b[35m";
	const char* GRAY = "\x1b[90m";
	const char* BOLD = "\x1b[1m";
}
char* getBlockColor(int type) {
	switch (type) {
	case 1: return "\x1b[31m";    // I 미노 red
	case 2: return "\x1b[32m";    // J 미노 green
	case 3: return "\x1b[33m";  // L 미노 yelloq
	case 4: return "\x1b[36m";  // O 미노 cyan
	case 5: return "\x1b[35m";   // S 미노 purple
	case 6: return  "\x1b[34m";  // T 미노
	case 7: return  "\x1b[37m";     // Z 미노
	case 8: return "\x1b[90m";    // 방해 블록(Garbage) 등 white
	default: return "\x1b[0m";  // 예외 처리
	}
}

void TerminalRenderer::setCursor(int x, int y) {
	printf("\033[%d;%dH", y + 1, x + 1);

}

void TerminalRenderer::clear() {

	printf("\033[2J\033[1;1H");
	fflush(stdout);
	printf("\e[?25l");
}

void TerminalRenderer::drawLogo() {
	// 각 글자를 배열로 분리
	hideCursor();
	const char* T[] = { "█████", "  █  ", "  █  ", "  █  ", "  █  " };
	const char* E[] = { "█████", "█    ", "███  ", "█    ", "█████" };
	const char* R[] = { "████ ", "█   █", "████ ", "█  █ ", "█   █" };
	const char* I[] = { "███", " █ ", " █ ", " █ ", "███" };
	const char* S[] = { " ████", "█    ", " ███ ", "    █", "████ " };
	const char* N[] = { "██  █", "█ █ █", "█  ██", "█   █", "█   █" };
	const char* M[] = { "█   █", "██ ██", "█ █ █", "█ █ █", "█   █" };
	const char* Y[] = { "█   █", "█   █", " █ █ ", "  █  ", "  █  " };


	int x = logoX; // 시작 X 좌표
	int y = 1;  // 시작 Y 좌표

	for (int i = 0; i < 5; i++) {
		setCursor(x, y + i);
		// 순서대로 색상과 함께 출력
		printf("%s%s ", Color::RED, T[i]); // T
		printf("%s%s ", Color::YELLOW, E[i]); // E
		printf("%s%s ", Color::GREEN, T[i]); // T
		printf("%s%s ", Color::CYAN, R[i]); // R
		printf("%s%s ", Color::PURPLE, I[i]); // I
		printf("%s%s ", Color::RED, S[i]); // S
		printf("%s%s ", Color::YELLOW, S[i]); // S
		printf("%s%s ", Color::GREEN, E[i]); // E
		printf("%s%s", Color::CYAN, N[i]); // N
		printf("%s", Color::RESET);
	}
	if (!isSingle)
	{
		for (int i = 0; i < 5; i++) {
			setCursor(78, y + i);
			// 순서대로 색상과 함께 출력

			printf("%s%s ", Color::GRAY, E[i]); // E
			printf("%s%s ", Color::GRAY, N[i]); // N
			printf("%s%s ", Color::GRAY, E[i]); // E
			printf("%s%s ", Color::GRAY, M[i]); // M
			printf("%s%s ", Color::GRAY, Y[i]); // Y

			printf("%s", Color::RESET);
		}
	}
	// v1 출력 (N 글자 아래쪽 끝에 배치)
	setCursor(x + 52, y + 4);
	printf("%s%s%s", Color::GRAY, "v1", Color::RESET);
}
void TerminalRenderer::renderBoard(const Board& board, const Tetromino& tetromino) {
	int startX = boardX;
	int startY = 7;

	// 1. 보드 데이터 가져오기 (int[22][10] 배열이라고 가정)
	auto game_board = board.get_board();

	// 2. 현재 떨어지는 미노 정보 가져오기
	auto [pos_r, pos_c] = tetromino.get_pos(); // 현재 위치 (Top-Left)
	const mino& shape = tetromino.get_shape(); // 현재 회전 상태의 4x4 배열 (int[4][4])

	setCursor(startX, startY);
	std::cout << Color::BOLD << "┏" << "━━━━━━━━━━━━━━━━━━━━━━" << "┓" << Color::RESET;

	// 행 루프 (2~21)
	for (int r = 2; r < 22; ++r) {
		setCursor(startX, startY + (r - 1));
		std::cout << Color::BOLD << "┃ " << Color::RESET;

		// 열 루프 (0~9)
		for (int c = 0; c < 10; ++c) {
			bool isFallingBlock = false;
			int blockType = 0;

			// --- [핵심 변경] 비트 연산 제거 -> 좌표 비교 ---

			// 1. 현재 좌표(r, c)가 떨어지는 미노의 4x4 영역 안에 있는지 확인
			bool insideBox = (r >= pos_r && r < pos_r + 4) && (c >= pos_c && c < pos_c + 4);

			if (insideBox) {
				// 4x4 배열 내부에서의 상대 좌표 계산 (0~3)
				int local_r = r - pos_r;
				int local_c = c - pos_c;

				// 해당 위치에 블록이 채워져 있는지 확인 (0이 아니면 블록임)
				if (shape[local_r][local_c] != 0) {
					isFallingBlock = true;
					// 미노 자체의 타입을 가져오거나, shape의 값을 사용
					blockType = tetromino.get_mino_type(); // 또는 shape[local_r][local_c]
				}
			}

			// --- 그리기 로직 ---

			if (isFallingBlock) {
				// 떨어지는 블록 그리기
				std::cout << getBlockColor(blockType) << "██" << Color::RESET;
			}
			else if (game_board[r][c] != 0) {
				// 바닥에 쌓인 블록 그리기
				std::cout << getBlockColor(game_board[r][c]) << "██" << Color::RESET;
			}
			else {
				// 빈 공간
				std::cout << Color::GRAY << ". " << Color::RESET;
			}
		}

		std::cout << Color::BOLD << " ┃" << Color::RESET;
	}

	if (!isSingle) renderEnemyBoard(board, tetromino);

	setCursor(startX, startY + 21);
	std::cout << Color::BOLD << "┗" << "━━━━━━━━━━━━━━━━━━━━━━" << "┛" << Color::RESET;
}

void TerminalRenderer::renderTimer(const int totalSec) {
	int min = (totalSec / 60);
	int sec = totalSec % 60;

	setCursor(middleX + 4, 4);

	std::cout << Color::BOLD;
	// 두 자리(setw(2))를 잡고, 빈 곳은 '0'으로 채움
	std::cout << std::setfill('0') << std::setw(2) << min % 100
		<< ":"
		<< std::setw(2) << sec;
	std::cout << Color::RESET;
}

void TerminalRenderer::renderNextBlock(const int* tetrominoArray) {
	for (size_t i = 0; i < 3; ++i) {
		//renderMinoPattern(middleX + 3, 11 + (i * 5), TETROMINO[tetrominoArray[i].get_shape(), Color::CYAN);
	}
}

void TerminalRenderer::renderHold(const Tetromino& tetromino) {

	if (tetromino.get_mino_type() < 0 || tetromino.get_mino_type() > 6) return;
	renderMinoPattern(7, 11, tetromino.get_shape(), getBlockColor(tetromino.get_mino_type()));
}

void TerminalRenderer::drawUIBox(std::string title, int x, int y, int w, int h, const char* color) {
	setCursor(x, y);
	std::cout << color << "┌" << std::string(w * 2, ' ') << "┐" << Color::RESET;
	for (int i = 1; i <= h; i++) {
		setCursor(x, y + i);
		std::cout << color << "│" << std::string(w * 2, ' ') << "│" << Color::RESET;
	}
	setCursor(x, y + h + 1);
	std::cout << color << "└" << std::string(w * 2, ' ') << "┘" << Color::RESET;
	if (!title.empty()) {
		setCursor(x + (w * 2 - title.length()) / 2, y);
		std::cout << color << Color::BOLD << "[" << title << "]" << Color::RESET;
	}
}
void TerminalRenderer::renderMinoPattern(int x, int y, const mino& shape, const char* color) {
	for (int i = 0; i < 4; i++) {
		setCursor(x, y + i);
		for (int j = 0; j < 4; j++) {
			if (shape[i][j] != 0) { // 배열 값이 0이 아니면 출력
				std::cout << color << "██" << Color::RESET;
			}
			else {
				std::cout << Color::GRAY << "  " << Color::RESET;
			}
		}
	}
}


void TerminalRenderer::renderBackground() {
	fflush(stdout);
	drawLogo();
	drawUIBox("HOLD", 4, 9, 6, 4, Color::GREEN);
	drawUIBox("NEXT", middleX, 9, 6, 15, Color::PURPLE);
	drawUIBox("#SCORE", 4, 16, 6, 3, Color::CYAN);
	drawUIBox("LV", 4, 21, 6, 3, Color::CYAN);
	drawUIBox("TIME", middleX, 2, 6, 3, Color::RESET);


}
void TerminalRenderer::drawHold(const mino& hold_shape) {
	renderMinoPattern(16, 3, hold_shape, Color::YELLOW);
}
void TerminalRenderer::renderScore(int score) {
	setCursor(8, 18); printf("%s%06d%s", Color::BOLD, score, Color::RESET);
}
void TerminalRenderer::renderLevel(int level) {
	setCursor(10, 23); printf("%s%02d%s", Color::BOLD, level, Color::RESET);
}
TerminalRenderer::~TerminalRenderer() {

}

void TerminalRenderer::hideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE; // FALSE로 설정하여 숨김
	SetConsoleCursorInfo(consoleHandle, &info);

}

void TerminalRenderer::showCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = TRUE; // 다시 보이게 설정
	SetConsoleCursorInfo(consoleHandle, &info);

}
void TerminalRenderer::renderGameOver() {
	setCursor(43, 16); printf("%s%s%s", Color::RED, "GAMEOVER", Color::RESET);
}

void TerminalRenderer::renderEnemyBoard(const Board& board, const Tetromino& tetromino) {
	int startX = boardX;
	int startY = 7;

	// 1. 보드 데이터 가져오기 (int[22][10] 배열이라고 가정)
	auto game_board = board.get_board();

	// 2. 현재 떨어지는 미노 정보 가져오기
	auto [pos_r, pos_c] = tetromino.get_pos(); // 현재 위치 (Top-Left)
	const mino& shape = tetromino.get_shape(); // 현재 회전 상태의 4x4 배열 (int[4][4])

	setCursor(startX, startY);
	std::cout << Color::BOLD << "┏" << "━━━━━━━━━━━━━━━━━━━━━━" << "┓" << Color::RESET;

	// 행 루프 (2~21)
	for (int r = 2; r < 22; ++r) {
		setCursor(startX, startY + (r - 1));
		std::cout << Color::BOLD << "┃ " << Color::RESET;

		// 열 루프 (0~9)
		for (int c = 0; c < 10; ++c) {
			bool isFallingBlock = false;
			int blockType = 0;

			// --- [핵심 변경] 비트 연산 제거 -> 좌표 비교 ---

			// 1. 현재 좌표(r, c)가 떨어지는 미노의 4x4 영역 안에 있는지 확인
			bool insideBox = (r >= pos_r && r < pos_r + 4) && (c >= pos_c && c < pos_c + 4);

			if (insideBox) {
				// 4x4 배열 내부에서의 상대 좌표 계산 (0~3)
				int local_r = r - pos_r;
				int local_c = c - pos_c;

				// 해당 위치에 블록이 채워져 있는지 확인 (0이 아니면 블록임)
				if (shape[local_r][local_c] != 0) {
					isFallingBlock = true;
					// 미노 자체의 타입을 가져오거나, shape의 값을 사용
					blockType = tetromino.get_mino_type(); // 또는 shape[local_r][local_c]
				}
			}

			// --- 그리기 로직 ---

			if (isFallingBlock) {
				// 떨어지는 블록 그리기
				std::cout << getBlockColor(blockType) << "██" << Color::RESET;
			}
			else if (game_board[r][c] != 0) {
				// 바닥에 쌓인 블록 그리기
				std::cout << getBlockColor(game_board[r][c]) << "██" << Color::RESET;
			}
			else {
				// 빈 공간
				std::cout << Color::GRAY << ". " << Color::RESET;
			}
		}

		std::cout << Color::BOLD << " ┃" << Color::RESET;
	}


	setCursor(startX, startY + 21);
	std::cout << Color::BOLD << "┗" << "━━━━━━━━━━━━━━━━━━━━━━" << "┛" << Color::RESET;

}