#ifndef __SOLO_ENGINE_HPP__
#define __SOLO_ENGINE_HPP__

#include "engine/engine.hpp"

class SoloEngine : public Engine {
    public:
    /**
     * @brief 게임 수행 전 필드 초기화(생성자)
     * @param 플랫폼 종속 인터페이스
     * @return 게임엔진
     */
    SoloEngine(Setting* setting, IInputHandler* input_handler, IRenderer* renderer);

    /**
     * @brief 게임 메인루프
     */
    void run() override;

    /**
     * @brief 게임 메인루프 정지
     */
    void stop() override;

    /**
     * @brief 게임 종료 시 메모리 및 기타 자원정리
     * @return 게임 비정상 종료 시 에러코드 반환
     */
    int finish() override;

    /**
     * @brief 게임 엔진 및 멤버 변수 정리(소멸자)
     */
    ~SoloEngine();
};

#endif
