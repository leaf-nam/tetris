#include "tetromino/tetromino.hpp"
#include "board/board.hpp"
#include "render/linux_render.hpp"

using namespace std;

static const string title = "TETRISSEN";

static const unordered_map<char, string> ascii_art = {
{
    'T',
    R"(
████████╗
╚══██╔══╝
   ██║   
   ██║   
   ██║   
   ╚═╝   
    )"
},
{
    'E',
    R"(
███████╗
██╔════╝
█████╗  
██╔══╝  
███████╗
╚══════╝
    )"
},
{
    'R',
    R"(
██████╗ 
██╔══██╗
██████╔╝
██╔══██╗
██║  ██║
╚═╝  ╚═╝
    )"
},
{
    'I',
    R"(
██╗
██║
██║
██║
██║
╚═╝
    )"
},
{
    'S',
    R"(
███████╗
██╔════╝
███████╗
╚════██║
███████║
╚══════╝
    )"
},
{
    'N',
    R"(
███╗   ██╗
████╗  ██║
██╔██╗ ██║
██║╚██╗██║
██║ ╚████║
╚═╝  ╚═══╝
    )"
}
};

static const char* ANSI_RESET = "\x1b[0m";

static unordered_map<int, string> color_map = {
    {MinoType::I,           "\x1b[38;2;139;233;253m"},
    {MinoType::O,           "\x1b[38;2;80;250;123m"},
    {MinoType::Z,           "\x1b[38;2;255;184;108m"},
    {MinoType::S,           "\x1b[38;2;255;121;198m"},
    {MinoType::J,           "\x1b[38;2;189;147;249m"},
    {MinoType::L,           "\x1b[38;2;255;85;85m"},
    {MinoType::T,           "\x1b[38;2;241;250;140m"},
    {MinoType::OBSTACLE,    "\x1b[38;2;98;114;164m"},
};

LinuxRender::LinuxRender()
{
    std::cout << "\x1b[?1049h";
    std::cout << "\x1b[2J\x1b[H";
    std::cout << "\x1b[?25l";
    std::cout << std::flush;
}

void LinuxRender::restoreTerminal()
{
    std::cout << "\x1b[?25h";
    std::cout << "\x1b[2J";
    std::cout << "\x1b[H";
    std::cout << "\x1b[?1049l";
    std::cout << std::flush;
}

LinuxRender::~LinuxRender()
{
    restoreTerminal();
}

void LinuxRender::go(int row, int col)
{
    cout << "\x1b[" << row + 1 << ";" << col + 1 << "H";
}

void LinuxRender::renderMino(int row, int col, const mino& tetromino, int type)
{
    int wr = row, wc = col;
    string color = color_map[type];

    go(wr, wc);
    for (int r = 0; r < MINO_SIZE; ++r) 
    {
        go(wr + r, wc);
        for (int c = 0; c < MINO_SIZE; ++c)
        {
            cout << (tetromino[r][c] ? color + "[]" : "  ") << ANSI_RESET;
        }
    }
}

/**
 * @brief 게임 로직과 무관한 배경 렌더링
 */
void LinuxRender::renderBackground() {
    int wr = 0, wc = 0;
    vector<vector<string>> lines;

    for (char c : title) 
    {
        string art = ascii_art.at(c);
        stringstream ss(art);
        string line;
        vector<string> v;
        while (getline(ss, line)) v.push_back(line);
        lines.push_back(v);
    }

    for (size_t row = 0; row < lines[0].size(); ++row) {
        go(wr + row, wc);
        for (size_t i = 0; i < title.size(); ++i) {
            const string& s = lines[i][row];
            if (!s.empty() && s.back() == '\n') cout << color_map[i % 7] << s.substr(0, s.size() - 1) << ANSI_RESET;
            else cout << color_map[i % 7] << s << ANSI_RESET;

            cout << "  ";
        }
    }

    cout << flush;
}
    
/**
 * @brief 게임판 렌더링
 * @param 보드 현상태 + 현재 테트로미노 렌더링
 */
void LinuxRender::renderBoard(const Board& board, const Tetromino& tetromino)
{
    string color;
    int wr = 7, wc = 10; // board 테두리 기준
    int br = wr + 1, bc = wc + 1; // board 내부 기준

    // board
    go(wr, wc);
    cout << "┌";
    for (int i = 0; i < BOARD_COL; ++i) cout << "──";
    cout << "┐";

    for (int r = BOARD_UPPER; r < BOARD_ROW; ++r) 
    {
        go(wr + 1 + r - BOARD_UPPER, wc);
        cout << "│";
        for (int c = 0; c < BOARD_COL; ++c)
        {
            color = color_map[board.at(r, c)];
            cout << (board.is_filled(r, c) ? color + "[]" : "  ") << ANSI_RESET;
        }
        cout << "│";
    }

    go(wr + 1 + (BOARD_ROW - BOARD_UPPER), wc);
    cout << "└";
    for (int i = 0; i < BOARD_COL; ++i) cout << "──";
    cout << "┘";

    // mino
    if (board.has_active_mino())
    {
        const auto [pos_r, pos_c] = tetromino.get_pos();
        const mino& m = tetromino.get_shape();
        int mino_type = tetromino.get_mino_type();
        const string color = color_map[mino_type];
        int mino_r, mino_c;

        go(br, bc);
        for (int r = BOARD_UPPER; r < BOARD_ROW; ++r) 
        {
            go(br + r - BOARD_UPPER, bc);
            mino_r = r - pos_r;
            for (int c = 0; c < BOARD_COL; ++c)
            {
                mino_c = c - pos_c;
                if (mino_r >= 0 && mino_r < MINO_SIZE && mino_c >= 0 && mino_c < MINO_SIZE) cout << (m[mino_r][mino_c] ? color + "[]" : "\x1b[2C") << ANSI_RESET;
                else cout << "\x1b[2C";
            }
        }       
    }

    cout << flush;
}

/**
 * @brief 타이머에 현재 시간 렌더링
 * @param 현재시간(초)
 */
void LinuxRender::renderTimer(const int sec)
{
    int wr = 19, wc = 32;

    int m = sec / 60, s = sec % 60;
    
    go(wr, wc);
    cout << "TIME";

    go(wr + 1, wc);
    cout << "     " << setw(2) << setfill('0') << m << ":" << setw(2) << setfill('0') << s;

    go(wr + 2, wc);
    cout << "──────────";
}

/**
 * @brief 다음 블럭 3개 렌더링
 * @param 다음 블럭 3개를 가진 배열 포인터(순서 중요)
 */
void LinuxRender::renderNextBlock(const int* tetrominoArray)
{
    int wr = 7, wc = 0;
    const mino& m1 = TETROMINO[tetrominoArray[0]][0];
    const mino& m2 = TETROMINO[tetrominoArray[1]][0];
    const mino& m3 = TETROMINO[tetrominoArray[2]][0];

    // border
    go(wr, wc);
    cout << "┌──NEXT──┐";

    for (int r = 0; r < 14; ++r)
    {
        go(wr + 1 + r, wc);
        cout << "│        │";
    }

    go (wr + 14, wc);
    cout << "└────────┘";

    // next mino
    renderMino(wr + 1, wc + 1, m1, tetrominoArray[0]);

    renderMino(wr + 5, wc + 1, m2, tetrominoArray[1]);

    renderMino(wr + 9, wc + 1, m3, tetrominoArray[2]);

    cout << flush;
}

/**
 * @brief 홀드할 블럭 렌더링
 * @param 홀드할 블럭
 */
void LinuxRender::renderHold(const Tetromino& tetromino)
{
    int wr = 7, wc = 32;
    int saved_mino_type = tetromino.get_mino_type();

    // border
    go(wr, wc);
    cout << "┌──HOLD──┐";

    for (int r = 0; r < 5; ++r)
    {
        go(wr + 1 + r, wc);
        cout << "│        │";
    }

    go (wr + 5, wc);
    cout << "└────────┘";

    if (saved_mino_type != -1)
    {
        const mino& m = TETROMINO[saved_mino_type][0];
        renderMino(wr + 1, wc + 1, m, saved_mino_type);
    }

    cout << flush;
}

void LinuxRender::renderOtherBoard(packet& pkt)
{
    string board_color;
    int wr = 7, wc = 50; // board 테두리 기준
    int br = wr - 1, bc = wc + 1; // board 내부 기준
    int pos_r = pkt.r;
    int pos_c = pkt.c;
    int mino_type = pkt.type;
    const mino& m = TETROMINO[mino_type][pkt.rotation];
    const string block_color = color_map[mino_type];
    int mino_r, mino_c;

    // board
    go(wr, wc);
    cout << "┌";
    for (int i = 0; i < BOARD_COL; ++i) cout << "──";
    cout << "┐";

    for (int r = BOARD_UPPER - 2; r < BOARD_ROW - 2; ++r) 
    {
        go(wr + 1 + r - (BOARD_UPPER - 2), wc);
        cout << "│";
        for (int c = 0; c < BOARD_COL; ++c)
        {
            board_color = color_map[pkt.board[r][c]];
            cout << (pkt.board[r][c] != MinoType::NONE ? board_color + "[]" : "  ") << ANSI_RESET;
        }
        cout << "│";
    }

    go(wr + 1 + (BOARD_ROW - BOARD_UPPER), wc);
    cout << "└";
    for (int i = 0; i < BOARD_COL; ++i) cout << "──";
    cout << "┘";

    // mino
    go(br, bc);
    for (int r = (BOARD_UPPER - 2); r < (BOARD_ROW - 2); ++r) 
    {
        if((br + r - (BOARD_UPPER - 2)) <= wr)
            continue;
        go(br + r - (BOARD_UPPER - 2), bc);
        mino_r = r - pos_r;
        for (int c = 0; c < BOARD_COL; ++c)
        {
            mino_c = c - pos_c;
            if (mino_r >= 0 && mino_r < MINO_SIZE && mino_c >= 0 && mino_c < MINO_SIZE) cout << (m[mino_r][mino_c] ? block_color + "[]" : "\x1b[2C") << ANSI_RESET;
            else cout << "\x1b[2C";
        }
    }       

    cout << flush;
}

void LinuxRender::renderIPRecv()
{
    cout << "대전 상대의 IP를 입력하세요: ";
    cout << flush;
}

void LinuxRender::renderChar(char c)
{
    cout << c;
    cout << flush;
}

void LinuxRender::renderClear()
{
    cout << "\033[2J\033[H";
    cout << flush;
}

/**
 * @brief 점수판 렌더링
 * @param 현재 점수
 */
void LinuxRender::renderScore(const int score)
{
    int wr = 13, wc = 32;
    
    go(wr, wc);
    cout << "SCORE";

    go(wr + 1, wc);
    cout << setw(10) << setfill(' ') << score;

    go(wr + 2, wc);
    cout << "──────────";
}

/**
 * @brief 레벨 렌더링
 * @param 현재 레벨
 */
void LinuxRender::renderLevel(const int level)
{
    int wr = 16, wc = 32;
    
    go(wr, wc);
    cout << "LEVEL";

    go(wr + 1, wc);
    cout << "       " << setw(3) << setfill('0') << level;

    go(wr + 2, wc);
    cout << "──────────";
}
