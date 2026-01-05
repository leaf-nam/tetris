# tetris
Nex1 Boot camp(3rd) Study Project

# 🎮 프로젝트: 멀티 테트리스
> **Linux 환경 기반의 로컬 네트워크 멀티플레이어 테트리스 게임**

<br>

## 🌿 브랜치 전략: Git Flow
- **`main`**: 실행 가능 최종 브랜치
- **`develop`**: 기능 통합 브랜치
- **`feature/(기능이름)`**: 세부 기능 개발 브랜치

<br>

## 🔄 참고용 워크플로우

기존 feat/tetris-name 에서 이어서 할 경우 (아직 로컬 develop 없는 경우) 세팅하기:
```bash
git fetch origin
git switch -c develop origin/develop
git switch feat/tetris-name
git merge develop
```

1. 로컬 `develop` 최신화
```bash
git switch develop
git pull origin develop
```

---
2. 새로운 `feature` 브랜치 생성
```bash
git switch -c feature/example
```

---
3. 코드 커밋

`git commit -m "[태그] 요약"` 형태로 작성
- `[Feat]`: 새로운 기능 추가
- `[Fix]`: 버그 수정
- `[Docs]`: 문서 수정 (README 등)
- `[Refactor]`: 코드 개선 (기능 변화 없음)
- `[Chore]`: 빌드 설정, 파일 이동 등 (기능 변화 없음)

**예시:** `git commit -m "[Feat] 타이머 구현"`

---
4. 깃헙에 완성된 `feature/example` 푸쉬
```bash
git push origin feature/example
```

---
5. `feature/example`->`develop`로 PR 

<br><br>

## 🧱코드 컨벤션 [(링크)](https://github.com/leaf-nam/tetris/wiki/Code-Convention)

- Class/Struct/Enum: PascalCase
- Object (Instances): camelCase
- Functions/Variables: snake_case
- Member variables: snake_case_
- Constants: SCREAMING_SNAKE_CASE
- File-name/Branch-name: kebab-case

<br>

## 📁디렉토리 [(링크)](https://github.com/leaf-nam/tetris/wiki/Directory-Architecture)

<br><br><br>

### Protection Rule Settings
- Require a pull request before merging
- Default develop ?
