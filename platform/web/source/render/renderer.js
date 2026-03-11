const BOARD_ROWS = 22;
const VISIBLE_ROWS = 20;
const HIDDEN_ROWS = BOARD_ROWS - VISIBLE_ROWS;
const COLS = 10;
const BOARD_CELL = 22;
const PREVIEW_CELL = 18;
const PREVIEW_SIZE = 4;
const NEXT_COUNT = 3;
const TAP_MAX_DURATION_MS = 220;
const TAP_MAX_DISTANCE_PX = 18;
const HOLD_DELAY_MS = 170;
const SWIPE_DISTANCE_PX = 54;
const HARD_DROP_SWIPE_PX = 92;
const HARD_DROP_MAX_DURATION_MS = 220;
const DRAG_STEP_PX = 26;
const SOFT_DROP_STEP_PX = 18;

const COLORS = [
  "#00BCD4",
  "#FFEB3B",
  "#F44336",
  "#4CAF50",
  "#3F51B5",
  "#FF9800",
  "#9C27B0",
  "#616161",
  "#111111"
];

class WebRenderer {
  constructor(moduleRef) {
    this.Module = moduleRef;
    this.boardCanvas = document.getElementById("game");
    this.boardCtx = this.boardCanvas.getContext("2d");

    this.holdCanvas = document.getElementById("hold");
    this.holdCtx = this.holdCanvas.getContext("2d");

    this.nextCanvases = Array.from({ length: NEXT_COUNT }, (_, index) => {
      const canvas = document.getElementById(`next${index}`);
      return { canvas, ctx: canvas.getContext("2d") };
    });

    this.timerEl = document.getElementById("timer");
    this.scoreEl = document.getElementById("score");
    this.levelEl = document.getElementById("level");
  }

  render_clear() {
    this._clearCanvas(this.boardCtx, this.boardCanvas);
    this._clearCanvas(this.holdCtx, this.holdCanvas);
    for (const preview of this.nextCanvases) {
      this._clearCanvas(preview.ctx, preview.canvas);
    }
  }

  render_background() {
    this._drawBoardBackground();
    this._drawPreviewBackground(this.holdCtx, this.holdCanvas);
    for (const preview of this.nextCanvases) {
      this._drawPreviewBackground(preview.ctx, preview.canvas);
    }
  }

  render_timer(totalSec) {
    const min = Math.floor(totalSec / 60);
    const sec = totalSec % 60;
    this.timerEl.textContent = `${String(min).padStart(2, "0")}:${String(sec).padStart(2, "0")}`;
  }

  render_score(score) {
    this.scoreEl.textContent = String(score);
  }

  render_level(level) {
    this.levelEl.textContent = String(level);
  }

  render_game_over() {
    this.boardCtx.save();
    this.boardCtx.fillStyle = "rgba(7, 10, 16, 0.72)";
    this.boardCtx.fillRect(0, 0, this.boardCanvas.width, this.boardCanvas.height);
    this.boardCtx.fillStyle = "#f7f3e8";
    this.boardCtx.font = "700 24px monospace";
    this.boardCtx.textAlign = "center";
    this.boardCtx.fillText("GAME OVER", this.boardCanvas.width / 2, this.boardCanvas.height / 2);
    this.boardCtx.restore();
  }

  render_board_from_heap() {
    const boardPtr = this.Module.ccall("get_board_ptr", "number", [], []);
    const shapePtr = this.Module.ccall("get_active_shape_ptr", "number", [], []);
    const metaPtr = this.Module.ccall("get_meta_ptr", "number", [], []);

    const board = this.Module.HEAP32.subarray(boardPtr >> 2, (boardPtr >> 2) + BOARD_ROWS * COLS);
    const shape = this.Module.HEAP32.subarray(shapePtr >> 2, (shapePtr >> 2) + PREVIEW_SIZE * PREVIEW_SIZE);
    const meta = this.Module.HEAP32.subarray(metaPtr >> 2, (metaPtr >> 2) + 5);

    const activeRow = meta[0];
    const activeCol = meta[1];
    const activeType = meta[3];
    const shadowOn = meta[4] === 1;

    this._drawBoardBackground();

    for (let row = HIDDEN_ROWS; row < BOARD_ROWS; row++) {
      for (let col = 0; col < COLS; col++) {
        const value = board[row * COLS + col];
        if (value >= 0 && value <= 7) {
          this._drawBoardCell(col, row, value);
        }
      }
    }

    if (shadowOn && activeType >= 0 && activeType <= 6) {
      const shadowCells = this._computeShadowCells(board, shape, activeRow, activeCol);
      for (const cell of shadowCells) {
        this._drawBoardCell(cell.col, cell.row, activeType, 0.28);
      }
    }

    if (activeType >= 0 && activeType <= 6) {
      for (let row = 0; row < PREVIEW_SIZE; row++) {
        for (let col = 0; col < PREVIEW_SIZE; col++) {
          if (shape[row * PREVIEW_SIZE + col] !== 0) {
            this._drawBoardCell(activeCol + col, activeRow + row, activeType);
          }
        }
      }
    }
  }

  render_hold_from_heap() {
    const shapePtr = this.Module.ccall("get_hold_shape_ptr", "number", [], []);
    const metaPtr = this.Module.ccall("get_hold_meta_ptr", "number", [], []);

    const shape = this.Module.HEAP32.subarray(shapePtr >> 2, (shapePtr >> 2) + PREVIEW_SIZE * PREVIEW_SIZE);
    const meta = this.Module.HEAP32.subarray(metaPtr >> 2, (metaPtr >> 2) + 1);

    this._drawPreviewBackground(this.holdCtx, this.holdCanvas);
    this._drawPreviewShape(this.holdCtx, this.holdCanvas, shape, meta[0]);
  }

  render_next_from_heap() {
    const shapesPtr = this.Module.ccall("get_next_shapes_ptr", "number", [], []);
    const typesPtr = this.Module.ccall("get_next_types_ptr", "number", [], []);

    const shapes = this.Module.HEAP32.subarray(
      shapesPtr >> 2,
      (shapesPtr >> 2) + NEXT_COUNT * PREVIEW_SIZE * PREVIEW_SIZE
    );
    const types = this.Module.HEAP32.subarray(typesPtr >> 2, (typesPtr >> 2) + NEXT_COUNT);

    for (let index = 0; index < NEXT_COUNT; index++) {
      const preview = this.nextCanvases[index];
      const start = index * PREVIEW_SIZE * PREVIEW_SIZE;
      const shape = shapes.subarray(start, start + PREVIEW_SIZE * PREVIEW_SIZE);
      this._drawPreviewBackground(preview.ctx, preview.canvas);
      this._drawPreviewShape(preview.ctx, preview.canvas, shape, types[index]);
    }
  }

  _clearCanvas(ctx, canvas) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
  }

  _drawBoardBackground() {
    this.boardCtx.fillStyle = "#080b12";
    this.boardCtx.fillRect(0, 0, this.boardCanvas.width, this.boardCanvas.height);

    this.boardCtx.strokeStyle = "#1d2634";
    this.boardCtx.lineWidth = 1;

    for (let row = 0; row <= VISIBLE_ROWS; row++) {
      const y = row * BOARD_CELL;
      this.boardCtx.beginPath();
      this.boardCtx.moveTo(0, y);
      this.boardCtx.lineTo(this.boardCanvas.width, y);
      this.boardCtx.stroke();
    }

    for (let col = 0; col <= COLS; col++) {
      const x = col * BOARD_CELL;
      this.boardCtx.beginPath();
      this.boardCtx.moveTo(x, 0);
      this.boardCtx.lineTo(x, this.boardCanvas.height);
      this.boardCtx.stroke();
    }
  }

  _drawPreviewBackground(ctx, canvas) {
    ctx.fillStyle = "#080b12";
    ctx.fillRect(0, 0, canvas.width, canvas.height);
    ctx.strokeStyle = "#1d2634";
    ctx.strokeRect(0.5, 0.5, canvas.width - 1, canvas.height - 1);
  }

  _drawBoardCell(col, boardRow, colorIndex, alpha = 1) {
    if (col < 0 || col >= COLS || boardRow < HIDDEN_ROWS || boardRow >= BOARD_ROWS) {
      return;
    }

    const x = col * BOARD_CELL;
    const y = (boardRow - HIDDEN_ROWS) * BOARD_CELL;
    this._drawCell(this.boardCtx, x, y, BOARD_CELL, colorIndex, alpha);
  }

  _drawPreviewShape(ctx, canvas, shape, type) {
    if (type < 0 || type > 6) {
      return;
    }

    let minRow = PREVIEW_SIZE;
    let maxRow = -1;
    let minCol = PREVIEW_SIZE;
    let maxCol = -1;

    for (let row = 0; row < PREVIEW_SIZE; row++) {
      for (let col = 0; col < PREVIEW_SIZE; col++) {
        if (shape[row * PREVIEW_SIZE + col] !== 0) {
          minRow = Math.min(minRow, row);
          maxRow = Math.max(maxRow, row);
          minCol = Math.min(minCol, col);
          maxCol = Math.max(maxCol, col);
        }
      }
    }

    if (maxRow === -1) {
      return;
    }

    const shapeWidth = (maxCol - minCol + 1) * PREVIEW_CELL;
    const shapeHeight = (maxRow - minRow + 1) * PREVIEW_CELL;
    const offsetX = Math.floor((canvas.width - shapeWidth) / 2);
    const offsetY = Math.floor((canvas.height - shapeHeight) / 2);

    for (let row = minRow; row <= maxRow; row++) {
      for (let col = minCol; col <= maxCol; col++) {
        if (shape[row * PREVIEW_SIZE + col] !== 0) {
          const x = offsetX + (col - minCol) * PREVIEW_CELL;
          const y = offsetY + (row - minRow) * PREVIEW_CELL;
          this._drawCell(ctx, x, y, PREVIEW_CELL, type);
        }
      }
    }
  }

  _drawCell(ctx, x, y, size, colorIndex, alpha = 1) {
    ctx.save();
    ctx.globalAlpha = alpha;
    ctx.fillStyle = COLORS[colorIndex] ?? "#ffffff";
    ctx.fillRect(x + 1, y + 1, size - 2, size - 2);
    ctx.strokeStyle = "rgba(255,255,255,0.18)";
    ctx.strokeRect(x + 1, y + 1, size - 2, size - 2);
    ctx.restore();
  }

  _computeShadowCells(board, shape, activeRow, activeCol) {
    let shadowRow = activeRow;
    while (!this._shapeCollides(board, shape, shadowRow + 1, activeCol)) {
      shadowRow += 1;
    }

    const cells = [];
    for (let row = 0; row < PREVIEW_SIZE; row++) {
      for (let col = 0; col < PREVIEW_SIZE; col++) {
        if (shape[row * PREVIEW_SIZE + col] !== 0) {
          cells.push({ row: shadowRow + row, col: activeCol + col });
        }
      }
    }
    return cells;
  }

  _shapeCollides(board, shape, baseRow, baseCol) {
    for (let row = 0; row < PREVIEW_SIZE; row++) {
      for (let col = 0; col < PREVIEW_SIZE; col++) {
        if (shape[row * PREVIEW_SIZE + col] === 0) {
          continue;
        }

        const boardRow = baseRow + row;
        const boardCol = baseCol + col;
        if (boardRow < 0 || boardRow >= BOARD_ROWS || boardCol < 0 || boardCol >= COLS) {
          return true;
        }

        const value = board[boardRow * COLS + boardCol];
        if (value >= 0 && value <= 7) {
          return true;
        }
      }
    }

    return false;
  }
}

function mapKeyToEngine(code) {
  switch (code) {
    case "ArrowLeft":
    case "KeyA":
      return "a".charCodeAt(0);
    case "ArrowRight":
    case "KeyD":
      return "d".charCodeAt(0);
    case "ArrowDown":
    case "KeyS":
      return "s".charCodeAt(0);
    case "ArrowUp":
    case "KeyE":
      return "e".charCodeAt(0);
    case "KeyQ":
      return "q".charCodeAt(0);
    case "Space":
    case "KeyF":
      return "f".charCodeAt(0);
    case "KeyW":
      return "w".charCodeAt(0);
    default:
      return 0;
  }
}

function sendEngineKey(Module, key) {
  if (!Module.ccall) {
    return;
  }

  Module.ccall("web_set_key", null, ["number"], [key]);
}

function sendEngineChar(Module, char) {
  sendEngineKey(Module, char.charCodeAt(0));
}

function findTouchById(touchList, identifier) {
  for (const touch of touchList) {
    if (touch.identifier === identifier) {
      return touch;
    }
  }
  return null;
}

function shouldCaptureGameInput(Module) {
  if (typeof Module.shouldCaptureGameInput === "function") {
    return Module.shouldCaptureGameInput() === true;
  }

  const gameScreen = document.getElementById("gameScreen");
  return !gameScreen || gameScreen.classList.contains("active");
}

function isTypingTarget(target) {
  if (!target) {
    return false;
  }

  const tag = target.tagName;
  return tag === "INPUT" || tag === "TEXTAREA" || tag === "SELECT" || target.isContentEditable;
}

function attachKeyboardControls(Module) {
  window.addEventListener("keydown", (event) => {
    if (!shouldCaptureGameInput(Module) || isTypingTarget(event.target)) {
      return;
    }

    const key = mapKeyToEngine(event.code);
    if (key === 0) {
      return;
    }

    event.preventDefault();
    sendEngineKey(Module, key);
  });
}

function attachTouchControls(Module) {
  const surface = Module.webRenderer.boardCanvas;
  let touchState = null;

  const clearHoldTimer = () => {
    if (touchState?.holdTimer) {
      window.clearTimeout(touchState.holdTimer);
      touchState.holdTimer = 0;
    }
  };

  const resetTouchState = () => {
    clearHoldTimer();
    touchState = null;
  };

  surface.addEventListener("touchstart", (event) => {
    if (!shouldCaptureGameInput(Module) || touchState || event.changedTouches.length === 0) {
      return;
    }

    const touch = event.changedTouches[0];
    event.preventDefault();

    touchState = {
      identifier: touch.identifier,
      startX: touch.clientX,
      startY: touch.clientY,
      currentX: touch.clientX,
      currentY: touch.clientY,
      dragAnchorX: touch.clientX,
      softDropAnchorY: touch.clientY,
      startTime: performance.now(),
      holdMode: false,
      consumed: false,
      holdTimer: 0
    };

    touchState.holdTimer = window.setTimeout(() => {
      if (!touchState || touchState.consumed) {
        return;
      }

      const dx = touchState.currentX - touchState.startX;
      const dy = touchState.currentY - touchState.startY;
      const distance = Math.hypot(dx, dy);
      if (distance <= TAP_MAX_DISTANCE_PX) {
        touchState.holdMode = true;
        touchState.dragAnchorX = touchState.currentX;
        touchState.softDropAnchorY = touchState.currentY;
      }
    }, HOLD_DELAY_MS);
  }, { passive: false });

  surface.addEventListener("touchmove", (event) => {
    if (!touchState) {
      return;
    }

    const touch = findTouchById(event.touches, touchState.identifier)
      ?? findTouchById(event.changedTouches, touchState.identifier);
    if (!touch) {
      return;
    }

    event.preventDefault();
    touchState.currentX = touch.clientX;
    touchState.currentY = touch.clientY;

    const dx = touchState.currentX - touchState.startX;
    const dy = touchState.currentY - touchState.startY;
    const elapsed = performance.now() - touchState.startTime;
    const isMostlyVertical = Math.abs(dy) > Math.abs(dx) * 1.15;

    if (!touchState.holdMode) {
      if (dy <= -SWIPE_DISTANCE_PX && isMostlyVertical) {
        touchState.consumed = true;
        sendEngineChar(Module, "w");
        resetTouchState();
        return;
      }

      if (dy >= HARD_DROP_SWIPE_PX && isMostlyVertical && elapsed <= HARD_DROP_MAX_DURATION_MS) {
        touchState.consumed = true;
        sendEngineChar(Module, "f");
        resetTouchState();
        return;
      }

      while (dy >= SOFT_DROP_STEP_PX && touchState.currentY - touchState.softDropAnchorY >= SOFT_DROP_STEP_PX) {
        clearHoldTimer();
        sendEngineChar(Module, "s");
        touchState.softDropAnchorY += SOFT_DROP_STEP_PX;
      }

      if (Math.hypot(dx, dy) > TAP_MAX_DISTANCE_PX) {
        clearHoldTimer();
      }
      return;
    }

    while (touchState.currentX - touchState.dragAnchorX >= DRAG_STEP_PX) {
      sendEngineChar(Module, "d");
      touchState.dragAnchorX += DRAG_STEP_PX;
    }

    while (touchState.currentX - touchState.dragAnchorX <= -DRAG_STEP_PX) {
      sendEngineChar(Module, "a");
      touchState.dragAnchorX -= DRAG_STEP_PX;
    }

    while (touchState.currentY - touchState.softDropAnchorY >= SOFT_DROP_STEP_PX) {
      sendEngineChar(Module, "s");
      touchState.softDropAnchorY += SOFT_DROP_STEP_PX;
    }
  }, { passive: false });

  const finishTouch = (event) => {
    if (!touchState) {
      return;
    }

    const touch = findTouchById(event.changedTouches, touchState.identifier);
    if (!touch) {
      return;
    }

    event.preventDefault();
    clearHoldTimer();

    if (!touchState.consumed) {
      const dx = touch.clientX - touchState.startX;
      const dy = touch.clientY - touchState.startY;
      const duration = performance.now() - touchState.startTime;
      const distance = Math.hypot(dx, dy);

      if (!touchState.holdMode && duration <= TAP_MAX_DURATION_MS && distance <= TAP_MAX_DISTANCE_PX) {
        sendEngineChar(Module, "e");
      }
    }

    touchState = null;
  };

  surface.addEventListener("touchend", finishTouch, { passive: false });
  surface.addEventListener("touchcancel", (event) => {
    if (touchState) {
      event.preventDefault();
    }
    resetTouchState();
  }, { passive: false });
}

export function attachRendererToModule(Module) {
  Module.webRenderer = new WebRenderer(Module);
  Module.webRenderer.render_background();

  attachKeyboardControls(Module);
  attachTouchControls(Module);
}


