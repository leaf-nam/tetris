![Build](https://github.com/leaf-nam/tetris/actions/workflows/build.yml/badge.svg)
![Coverage](https://codecov.io/gh/leaf-nam/tetris/branch/develop/graph/badge.svg)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg?logo=c%2B%2B)
![C#](https://img.shields.io/badge/C%23-.NET-blueviolet.svg?logo=csharp)

### 📢 Branch Maintenance Notice
main, develop 제외한 나머지 브랜치 리무브 예정!

</br>
</br>
</br>

# TETRISSEN: Cross-Platform Multiplayer Tetris

**TETRISSEN**은 C++ 기반으로 구현된 **크로스 플랫폼 테트리스 게임**입니다.  
게임 로직과 렌더링을 분리한 클린 아키텍처(Clean Architecture)를 적용하여,   
서로 다른 플랫폼에서 공통 API를 통해 동일한 게임 로직을 재사용할 수 있도록 설계되었습니다.

**Supported Platforms:**
Windows (WPF, Terminal) · Linux (Terminal) · Web *(추가예정)*

---

## 🕹️ 게임 모드

### 1️⃣ 싱글 플레이어
*(스크린샷 추가 예정)*  
![Gameplay Screenshot]()

- 일정 시간이 지나면 레벨 상승
- 레벨 증가 시 하단에 방해 블록 라인 추가
- 레벨 증가 시 블록 낙하 속도 증가

---

### 2️⃣ 멀티 플레이어
*(스크린샷 추가 예정)*  
![Gameplay Screenshot]()

- IP 주소를 통한 방 생성 / 참가, 실시간 대전 플레이
- 2줄 이상 제거 시 상대방 보드 하단에 방해 블록 라인 추가하여 공격

---

## 조작 방식 (Keyboard Controls)

| Key | Action |
|---|---|
| A | 왼쪽 이동 |
| D | 오른쪽 이동 |
| S | 소프트 드롭 |
| F | 하드 드롭 |
| E | 시계 방향 회전 |
| Q | 반시계 방향 회전 |
| W | 홀드 블록 스왑 |

---

## 게임 기능

- 기본 테트리스 로직
  - Soft Drop
  - Hard Drop
  - Tuck Move
  - Wall Kick (Rotation System)
- 다음 블록 미리보기 (3개)
- 블록 홀드 시스템 (1개)
- 타이머 보드
- 스코어 보드

---

## ⚙️ 옵션 설정

- 블록 테마 변경
- 고스트 블록 ON / OFF
