#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "api/i_input_handler.hpp"
#include "api/i_renderer.hpp"

class Engine {
    private:
    IInputHandler* input_handler;
    IRenderer* renderer;    

    public:
    /**
     * @brief 게임 수행 전 필드 초기화(생성자)
     * @param 핸들러, 렌더러 인터페이스
     * @return 게임엔진
     */
<<<<<<< HEAD
    Engine(IInputHandler*, IRenderer*);
=======
    Engine(IInputHandler* input_handler, IRenderer* renderer);
>>>>>>> b76dff269f0c7cc36325ec90d47dc53b31626446

    /**
     * @brief 게임 메인루프
     */
    void run();

    /**
     * @brief 게임 메인루프 정지
     */
    void stop();

    /**
     * @brief 게임 종료 시 메모리 및 기타 자원정리
     * @return 게임 비정상 종료 시 에러코드 반환
     */
    int finish();

    /**
     * @brief 게임 엔진 및 멤버 변수 정리(소멸자)
     */
    ~Engine();
};

#endif
