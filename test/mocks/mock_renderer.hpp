#include "i_renderer.hpp"

class MockRenderer : public IRenderer
{
public:
    int key = 0;

    /**
     * @brief 게임 로직과 무관한 배경 렌더링
     */
    void render_background() override {}
    
    /**
     * @brief 게임판 렌더링
     * @param 보드 현상태 + 현재 테트로미노 렌더링
     */
    void render_board(const Board& board, const Tetromino& tetromino) override {}

    /**
     * @brief 타이머에 현재 시간 렌더링
     * @param 현재시간(초)
     */
    void render_timer(int sec) override {}

    /**
     * @brief 다음 블럭 3개 렌더링
     * @param 다음 블럭 3개를 가진 배열 포인터(순서 중요)
     */
    void render_next_block(const int* tetrominoArray) override {}

    /**
     * @brief 홀드할 블럭 렌더링
     * @param 홀드할 블럭
     */
    void render_hold(const Tetromino& tetromino) override {}

    /**
     * @brief 점수판 렌더링
     * @param 현재 점수
     */
    void render_score(int score) override {}

    /**
     * @brief 레벨 렌더링
     * @param 현재 레벨
     */
    void render_level(int level) override {}
    
    void render_other_board(Packet& pkt) override {}

    void render_ip_recv() override {}
    
    void render_char(char c) override {}
    
    void render_clear() override {}
};