#include "tetromino/tetromino.hpp"

using namespace std;

Tetromino::Tetromino() : mino_type(0), rotation(0), pos(make_pair(0, 3)) {}

/**
 * @brief 테트로미노를 초기화
 * @param type 테트로미노 타입
 */
void Tetromino::init_mino(int type)
{
    mino_type = type;
    pos.first = 0;
    pos.second = 3;
    rotation = 0;
}

/**
 * @brief 테트로미노 회전
 * @param rot 회전 방향
 */
void Tetromino::set_rotation(int rot) 
{
    rotation = rot;
}

/**
 * @brief 테트로미노 평행 이동
 * @param dir 이동 방향
 */
void Tetromino::set_pos(int new_r, int new_c) 
{
    pos.first = new_r;
    pos.second = new_c;
}

/**
 * @brief 테트로미노 위치 (기준: 좌상단)
 * @return 테트로미노 위치
 */
pair<int, int> Tetromino::get_pos()
{
    return pos;
}

/**
 * @brief 테트로미노 모양
 * @return 테트로미노 모양
 */
mino Tetromino::get_shape()
{
    return TETROMINO[mino_type][rotation];
}

/**
 * @brief 회전한 테트로미노 모양을 반환하는 함수. 회전 상태가 반영되진 않음
 * @return 테트로미노 모양
 */
mino Tetromino::get_shape(int rot)
{
    return TETROMINO[mino_type][rot];
}

int Tetromino::get_rotation()
{
    return rotation;
}