const ROWS = 22;
const COLS = 10;
const CELL = 22;

const COLORS = [
  "#00BCD4", // I
  "#FFEB3B", // O
  "#F44336", // Z
  "#4CAF50", // S
  "#3F51B5", // J
  "#FF9800", // L
  "#9C27B0", // T
  "#616161", // OBSTACLE
  "#111111"  // NONE
];

class WebRenderer {
  constructor(moduleRef) {
    this.Module = moduleRef;
    this.canvas = document.getElementById("game");
    this.ctx = this.canvas.getContext("2d");

    this.timerEl = document.getElementById("timer");
    this.scoreEl = document.getElementById("score");
    this.levelEl = document.getElementById("level");
  }

  render_clear() {
    this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
  }

  render_background() {
    this.ctx.fillStyle = "#0b0b0f";
    this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);

    this.ctx.strokeStyle = "#1b1b22";
    this.ctx.lineWidth = 1;
    for (let r = 0; r <= ROWS; r++) {
      const y = r * CELL;
      this.ctx.beginPath();
      this.ctx.moveTo(0, y);
      this.ctx.lineTo(COLS * CELL, y);
      this.ctx.stroke();
    }
    for (let c = 0; c <= COLS; c++) {
      const x = c * CELL;
      this.ctx.beginPath();
      this.ctx.moveTo(x, 0);
      this.ctx.lineTo(x, ROWS * CELL);
      this.ctx.stroke();
    }
  }

  render_timer(sec) {
    if (this.timerEl) this.timerEl.textContent = String(sec);
  }

  render_score(score) {
    if (this.scoreEl) this.scoreEl.textContent = String(score);
  }

  render_level(level) {
    if (this.levelEl) this.levelEl.textContent = String(level);
  }

  render_game_over() {
    this.ctx.fillStyle = "rgba(0,0,0,0.6)";
    this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);
    this.ctx.fillStyle = "#ffffff";
    this.ctx.font = "bold 24px sans-serif";
    this.ctx.textAlign = "center";
    this.ctx.fillText("GAME OVER", this.canvas.width / 2, this.canvas.height / 2);
  }

  _drawCell(c, r, colorIndex) {
    const x = c * CELL;
    const y = r * CELL;

    this.ctx.fillStyle = COLORS[colorIndex] ?? "#ffffff";
    this.ctx.fillRect(x + 1, y + 1, CELL - 2, CELL - 2);

    this.ctx.strokeStyle = "rgba(255,255,255,0.15)";
    this.ctx.strokeRect(x + 1, y + 1, CELL - 2, CELL - 2);
  }

  render_board_from_heap() {
    const boardPtr = this.Module.ccall("get_board_ptr", "number", [], []);
    const shapePtr = this.Module.ccall("get_active_shape_ptr", "number", [], []);
    const metaPtr = this.Module.ccall("get_meta_ptr", "number", [], []);

    const board = this.Module.HEAP32.subarray(boardPtr >> 2, (boardPtr >> 2) + ROWS * COLS);
    const shape = this.Module.HEAP32.subarray(shapePtr >> 2, (shapePtr >> 2) + 16);
    const meta = this.Module.HEAP32.subarray(metaPtr >> 2, (metaPtr >> 2) + 4);

    const ar = meta[0];
    const ac = meta[1];
    const type = meta[3];

    this.render_background();

    for (let r = 0; r < ROWS; r++) {
      for (let c = 0; c < COLS; c++) {
        const v = board[r * COLS + c];
        if (v >= 0 && v <= 7) this._drawCell(c, r, v);
      }
    }

    if (type >= 0 && type <= 6) {
      for (let r = 0; r < 4; r++) {
        for (let c = 0; c < 4; c++) {
          if (shape[r * 4 + c] !== 0) {
            const br = ar + r;
            const bc = ac + c;
            if (br >= 0 && br < ROWS && bc >= 0 && bc < COLS) {
              this._drawCell(bc, br, type);
            }
          }
        }
      }
    }
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

export function attachRendererToModule(Module) {
  Module.webRenderer = new WebRenderer(Module);

  window.addEventListener("keydown", (e) => {
    const key = mapKeyToEngine(e.code);
    if (key === 0) return;

    e.preventDefault();
    if (Module.ccall) {
      Module.ccall("web_set_key", null, ["number"], [key]);
    }
  });
}
