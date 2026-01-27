#include "../../../core/source/engine/SoloEngine.hpp"

#include <emscripten/bind.h>
#include <vector>
// 필요한 헤더들 include

using namespace emscripten;

// JS와 소통하기 위한 래퍼(Wrapper) 클래스
class TetrisWasm
{
  private:
    SoloEngine* engine;
    // (참고: 엔진 생성자가 복잡하다면 여기서 더미(Dummy) 객체를 넣어줘야 할 수도 있습니다)

  public:
    TetrisWasm()
    {
        // JS에서는 복잡한 의존성 주입이 어려우니, 여기서 엔진을 조립합니다.
        // 렌더러와 인풋핸들러는 C++쪽에서는 필요 없으므로 nullptr나 더미를 넣습니다.
        // 예시: engine = new SoloEngine(new Setting(), nullptr, nullptr);

        // 주의: SoloEngine 내부에서 nullptr 체크를 안 한다면
        // 빈 껍데기(Dummy) 클래스를 만들어서 넣어줘야 합니다.
    }

    // 1. 게임 로직 한 프레임 진행 (JS가 호출함)
    void update()
    {
        if (engine) engine->update(); // 엔진의 update() 호출
    }

    // 2. 키 입력 전달 (JS -> C++)
    void input(int keyCode)
    {
        // 예: JS의 키코드를 엔진의 키값으로 변환해서 전달
        // engine->onInput(keyCode);
    }

    // 3. 보드 데이터 반환 (C++ -> JS)
    // 2차원 배열을 1차원 벡터로 펴서 보내는 게 JS에서 받기 편합니다.
    std::vector<int> getBoard()
    {
        std::vector<int> boardData;
        // 엔진에서 보드 정보를 가져와서 boardData에 채워넣는 로직
        // for(int y=0; y<20; y++) {
        //     for(int x=0; x<10; x++) {
        //         boardData.push_back(engine->getBoardCell(x, y));
        //     }
        // }
        return boardData;
    }
};

// ★ 여기가 핵심: JS로 내보내기
EMSCRIPTEN_BINDINGS(my_module)
{
    register_vector<int>("VectorInt"); // vector<int>를 JS 배열로 변환 허용

    class_<TetrisWasm>("TetrisGame") // JS에서는 'TetrisGame'이라는 이름으로 씀
        .constructor<>()
        .function("update", &TetrisWasm::update)
        .function("input", &TetrisWasm::input)
        .function("getBoard", &TetrisWasm::getBoard);
}