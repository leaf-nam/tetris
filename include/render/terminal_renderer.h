#include<iostream>

class IRenderer {
public:
    virtual ~IRenderer() {}
    virtual bool initialize() = 0; // 창 생성, 라이브러리 초기화
    virtual void clear() = 0;      // 화면 지우기
    virtual void draw(const Board& board, const Vector<Tetrimino>& queue) = 0; // 그리기 호출
    virtual void display() = 0;    // 최종적으로 화면에 표시
};