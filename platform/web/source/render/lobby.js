const STORAGE_KEYS = {
  nickname: "tetrissen.nickname",
  clientId: "tetrissen.client-id",
  peerAddress: "tetrissen.peer-address"
};

const LOBBY_PACKET_CHANNEL_NAME = "tetrissen-lobby-packet-v1";
const APP_STATE = {
  MENU: 0,
  SINGLE_PLAY: 1,
  LOBBY: 2,
  MULTI_SERVER: 3,
  MULTI_CLIENT: 4,
  SETTINGS: 5,
  EXIT: 6
};

const DEBUG_LOBBY = true;

function lobbyDebug(...args) {
  if (!DEBUG_LOBBY) {
    return;
  }
  console.log("[web-lobby]", ...args);
}

function getOrCreateClientId() {
  const saved = sessionStorage.getItem(STORAGE_KEYS.clientId);
  if (saved) {
    return saved;
  }

  const value = (crypto.randomUUID ? crypto.randomUUID() : `client-${Date.now()}-${Math.floor(Math.random() * 100000)}`)
    .replace(/[^a-zA-Z0-9-]/g, "")
    .slice(0, 24);
  sessionStorage.setItem(STORAGE_KEYS.clientId, value);
  return value;
}

function getOrCreatePeerAddress() {
  const saved = sessionStorage.getItem(STORAGE_KEYS.peerAddress);
  if (saved) {
    return saved.slice(0, 15);
  }

  const value = Math.random().toString(36).slice(2, 10).padEnd(8, "0").slice(0, 8);
  sessionStorage.setItem(STORAGE_KEYS.peerAddress, value);
  return value;
}

function createWebLobbyNetwork(Module) {
  const state = {
    address: getOrCreatePeerAddress(),
    channel: null,
    userQueue: [],
    roomQueue: []
  };

  function ensureChannel() {
    if (state.channel) {
      return state.channel;
    }

    if (!("BroadcastChannel" in window)) {
      return null;
    }

    state.channel = new BroadcastChannel(LOBBY_PACKET_CHANNEL_NAME);
    lobbyDebug("channel-open", { address: state.address, channel: LOBBY_PACKET_CHANNEL_NAME });

    state.channel.addEventListener("message", (event) => {
      const message = event.data;
      lobbyDebug("channel-message", { self: state.address, message });

      if (!message || message.source === state.address) {
        lobbyDebug("channel-skip-self-or-empty", { self: state.address, message });
        return;
      }

      const target = String(message.target || "");
      if (target !== "broadcast" && target !== state.address) {
        lobbyDebug("channel-skip-target-mismatch", { self: state.address, target, message });
        return;
      }

      if (message.kind === "user") {
        lobbyDebug("queue-user", { self: state.address, from: message.source, payload: message.payload });
        state.userQueue.push({
          ip: String(message.source || "").slice(0, 15),
          id: String(message.payload?.id || "").slice(0, 8),
          isEnter: message.payload?.isEnter === true
        });
        return;
      }

      if (message.kind === "room") {
        const ids = Array.isArray(message.payload?.ids)
          ? message.payload.ids.map((id) => String(id || "").slice(0, 8)).slice(0, 4)
          : [];
        lobbyDebug("queue-room", { self: state.address, from: message.source, payload: message.payload });
        state.roomQueue.push({
          ip: String(message.source || "").slice(0, 15),
          roomMasterId: String(message.payload?.roomMasterId || "").slice(0, 8),
          ids,
          roomName: String(message.payload?.roomName || "").slice(0, 8),
          idLen: Number(message.payload?.idLen || ids.length),
          isEnterNotSuccess: message.payload?.isEnterNotSuccess === true,
          isGameStart: message.payload?.isGameStart === true,
          isBroadcast: message.payload?.isBroadcast === true,
          isUpdate: message.payload?.isUpdate === true,
          isBroadcastDelete: message.payload?.isBroadcastDelete === true
        });
      }
    });

    return state.channel;
  }

  function post(kind, target, payload) {
    const channel = ensureChannel();
    if (!channel) {
      return;
    }

    const message = {
      kind,
      source: state.address,
      target: String(target || "broadcast").slice(0, 15),
      payload
    };
    lobbyDebug("channel-post", message);
    channel.postMessage(message);
  }

  ensureChannel();

  Module.webLobbyNetwork = {
    sendUserData({ id, isEnter, sendIp }) {
      post("user", sendIp, {
        id: String(id || "").slice(0, 8),
        isEnter: isEnter === true
      });
    },
    pollUserData() {
      ensureChannel();
      const packet = state.userQueue.shift() || null;
      lobbyDebug("poll-user", { self: state.address, packet, remaining: state.userQueue.length });
      return packet;
    },
    sendRoomData(packet) {
      post("room", packet.sendIp, {
        roomMasterId: String(packet.roomMasterId || "").slice(0, 8),
        ids: Array.isArray(packet.ids) ? packet.ids.map((id) => String(id || "").slice(0, 8)).slice(0, 4) : [],
        roomName: String(packet.roomName || "").slice(0, 8),
        idLen: Number(packet.idLen || 0),
        isEnterNotSuccess: packet.isEnterNotSuccess === true,
        isGameStart: packet.isGameStart === true,
        isBroadcast: packet.isBroadcast === true,
        isUpdate: packet.isUpdate === true,
        isBroadcastDelete: packet.isBroadcastDelete === true
      });
    },
    pollRoomData() {
      ensureChannel();
      const packet = state.roomQueue.shift() || null;
      lobbyDebug("poll-room", { self: state.address, packet, remaining: state.roomQueue.length });
      return packet;
    }
  };
}

export function attachLobbyUi(Module) {
  lobbyDebug("attach-ui");
  createWebLobbyNetwork(Module);
  lobbyDebug("network-ready", { hasNetwork: !!Module.webLobbyNetwork });

  const screens = {
    menu: document.getElementById("menuScreen"),
    lobby: document.getElementById("lobbyScreen"),
    game: document.getElementById("gameScreen")
  };

  const panes = {
    nickname: document.getElementById("nicknamePane"),
    entrance: document.getElementById("entrancePane"),
    create: document.getElementById("createRoomPane"),
    browse: document.getElementById("browsePane"),
    waiting: document.getElementById("waitingPane")
  };

  const menuOptions = Array.from(document.querySelectorAll(".menu-option"));
  const entranceOptions = Array.from(document.querySelectorAll(".entrance-option"));
  const roomList = document.getElementById("roomList");
  const nicknameInput = document.getElementById("nicknameInput");
  const roomNameInput = document.getElementById("roomNameInput");
  const nicknameCurrent = document.getElementById("nicknameCurrent");
  const waitingHint = document.getElementById("waitingHint");
  const waitingRoomName = document.getElementById("waitingRoomName");
  const waitingHostName = document.getElementById("waitingHostName");
  const waitingGuestList = document.getElementById("waitingGuestList");
  const lobbyNote = document.getElementById("lobbyNote");
  const btnBackMenu = document.getElementById("btnBackMenu");
  const btnStart = document.getElementById("btnStart");
  const btnReset = document.getElementById("btnReset");

  const state = {
    menuIndex: 0,
    pane: "nickname",
    keyQueue: [],
    textQueue: [],
    drivenLobby: false,
    lobbyRunning: false,
    rooms: [],
    roomName: "",
    hostName: "",
    isServer: false,
    roomUsers: [],
    nickname: (localStorage.getItem(STORAGE_KEYS.nickname) || "GUEST").slice(0, 8),
    clientId: getOrCreateClientId()
  };

  Module.shouldCaptureGameInput = () => screens.game.classList.contains("active");

  function setLobbyNote(message) {
    lobbyNote.textContent = message;
  }

  function showScreen(name) {
    Object.entries(screens).forEach(([key, element]) => {
      element.classList.toggle("active", key === name);
    });
  }

  function renderMenuSelection() {
    menuOptions.forEach((option, index) => {
      const active = index === state.menuIndex;
      option.classList.toggle("active", active);
      option.setAttribute("aria-selected", active ? "true" : "false");
    });
  }

  function renderEntranceOptions(selected = 0) {
    entranceOptions.forEach((option, index) => {
      option.classList.toggle("active", index === selected);
    });
  }

  function showPane(name) {
    state.pane = name;
    Object.entries(panes).forEach(([key, element]) => {
      element.classList.toggle("active", key === name);
    });

    if (name === "nickname") {
      nicknameCurrent.textContent = `Current Nickname : [ ${state.nickname || "GUEST"} ]`;
      nicknameInput.value = state.nickname === "GUEST" ? "" : state.nickname;
      requestAnimationFrame(() => nicknameInput.focus());
    } else if (name === "create") {
      roomNameInput.value = "";
      requestAnimationFrame(() => roomNameInput.focus());
    }
  }

  function pauseGame() {
    if (Module.ccall) {
      Module.ccall("game_pause", null, [], []);
    }
  }

  function showMenu() {
    state.drivenLobby = false;
    showScreen("menu");
    renderMenuSelection();
  }

  function renderRoomList() {
    if (state.rooms.length === 0) {
      roomList.innerHTML = '<div class="room-empty">No rooms found yet. Wait for a host to broadcast a room.</div>';
      return;
    }

    roomList.innerHTML = "";
    state.rooms.forEach((room, index) => {
      const row = document.createElement("div");
      row.className = `room-row${room.selected ? " active" : ""}`;
      row.textContent = `[ ${room.roomName} ]  HOST : ${room.ip}`;
      roomList.appendChild(row);
    });
  }

  function renderWaitingRoom() {
    waitingHint.textContent = state.isServer
      ? "> Start Game : Press [ SPACE ]    > Delete Room : Press [ ESC ]"
      : "> Out Room : Press [ ESC ]";
    waitingRoomName.textContent = state.roomName ? `ROOM NAME : [ ${state.roomName} ]` : "";
    waitingHostName.textContent = state.hostName ? `HOST : [ ${state.hostName} ]` : "";
    waitingGuestList.innerHTML = "";

    state.roomUsers.forEach((user) => {
      const line = document.createElement("div");
      line.textContent = `- ${user}`;
      waitingGuestList.appendChild(line);
    });
  }

  function queueKey(key) {
    const queued = key | 0;
    lobbyDebug("queue-key", { key: queued, pane: state.pane });
    state.keyQueue.push(queued);
  }

  function queueText(text) {
    const value = String(text || "").trim().slice(0, 8);
    if (!value) {
      return;
    }

    lobbyDebug("queue-text", { value, pane: state.pane });
    state.textQueue.push(value);
  }

  async function runLobbyFlow() {
    lobbyDebug("run-lobby-flow:start", { lobbyRunning: state.lobbyRunning, hasCcall: !!Module.ccall });
    if (state.lobbyRunning || !Module.ccall) {
      return;
    }

    pauseGame();
    showScreen("lobby");
    setLobbyNote("Lobby keys are handled by the shared C++ lobby flow.");
    state.keyQueue.length = 0;
    state.textQueue.length = 0;
    state.lobbyRunning = true;

    try {
      const result = await Module.ccall("web_run_lobby", "number", [], [], { async: true });
      lobbyDebug("run-lobby-flow:end", { result });
    } finally {
      state.lobbyRunning = false;
    }
  }

  function startSinglePlay() {
    showScreen("game");
    if (Module.ccall) {
      Module.ccall("game_reset", null, [], []);
    }
  }

  async function handleMenuAction(action) {
    lobbyDebug("menu-action", { action });
    switch (action) {
      case "single":
        startSinglePlay();
        break;
      case "multi":
        await runLobbyFlow();
        break;
      case "settings":
        alert("Web settings UI is not connected yet.");
        break;
      case "exit":
        pauseGame();
        showMenu();
        break;
    }
  }

  Module.webLobbyUi = {
    begin() {
      state.drivenLobby = true;
      state.keyQueue.length = 0;
      state.textQueue.length = 0;
      showScreen("lobby");
      setLobbyNote("Use arrow keys and space. Enter submits nickname or room name.");
    },
    finish(result) {
      state.drivenLobby = false;
      if (result === APP_STATE.MULTI_SERVER || result === APP_STATE.MULTI_CLIENT) {
        setLobbyNote("Room connection succeeded. Web in-game multiplayer is not wired yet.");
        showScreen("lobby");
        return;
      }
      showMenu();
    },
    renderSetNickname(nickname) {
      state.nickname = String(nickname || "").slice(0, 8) || "GUEST";
      localStorage.setItem(STORAGE_KEYS.nickname, state.nickname);
      showPane("nickname");
    },
    renderEntrance() {
      showPane("entrance");
      renderEntranceOptions(0);
    },
    renderEntranceChoice(entrance) {
      showPane("entrance");
      renderEntranceOptions(entrance | 0);
    },
    renderCreateRoom() {
      showPane("create");
      setLobbyNote("Enter the room name and press Enter.");
    },
    renderRoom(roomName, hostName, isServer) {
      state.roomName = String(roomName || "").slice(0, 8);
      state.hostName = String(hostName || "").slice(0, 8);
      state.isServer = isServer === true;
      showPane("waiting");
      renderWaitingRoom();
    },
    renderRoomClients(serialized) {
      state.roomUsers = String(serialized || "")
        .split("\n")
        .map((value) => value.trim())
        .filter(Boolean);
      renderWaitingRoom();
    },
    renderLobby() {
      showPane("browse");
      setLobbyNote("Browse detected rooms with arrow keys and join with Space.");
    },
    renderLobbyRooms(serialized, selected) {
      const rows = String(serialized || "")
        .split("\n")
        .filter(Boolean)
        .map((line, index) => {
          const [roomName, ip] = line.split("|");
          return {
            roomName: String(roomName || "ROOM").slice(0, 8),
            ip: String(ip || "").slice(0, 15),
            selected: index === (selected | 0)
          };
        });
      state.rooms = rows;
      renderRoomList();
    },
    renderUserIdInput() {
      setLobbyNote("Waiting for room broadcasts...");
    },
    renderClear() {},
    consumeKey() {
      const key = state.keyQueue.shift() || 0;
      if (key !== 0) lobbyDebug("consume-key", { key, pane: state.pane });
      return key;
    },
    consumeText() {
      const text = state.textQueue.shift() || "";
      if (text) lobbyDebug("consume-text", { text, pane: state.pane });
      return text;
    }
  };

  menuOptions.forEach((option, index) => {
    option.addEventListener("click", async () => {
      state.menuIndex = index;
      renderMenuSelection();
      await handleMenuAction(option.dataset.action);
    });
  });

  btnBackMenu.addEventListener("click", () => {
    pauseGame();
    showMenu();
  });

  btnStart.addEventListener("click", () => {
    if (Module.ccall) {
      Module.ccall("game_start", null, [], []);
    }
  });

  btnReset.addEventListener("click", () => {
    if (Module.ccall) {
      Module.ccall("game_reset", null, [], []);
    }
  });

  window.addEventListener("keydown", async (event) => {
    if (screens.game.classList.contains("active")) {
      return;
    }

    if (screens.menu.classList.contains("active")) {
      if (event.key === "ArrowDown" || event.key === "Tab") {
        event.preventDefault();
        state.menuIndex = (state.menuIndex + 1) % menuOptions.length;
        renderMenuSelection();
      } else if (event.key === "ArrowUp") {
        event.preventDefault();
        state.menuIndex = (state.menuIndex - 1 + menuOptions.length) % menuOptions.length;
        renderMenuSelection();
      } else if (event.key === "Enter" || event.key === " ") {
        event.preventDefault();
        await handleMenuAction(menuOptions[state.menuIndex].dataset.action);
      }
      return;
    }

    if (!screens.lobby.classList.contains("active") || !state.drivenLobby) {
      return;
    }

    if (state.pane === "nickname") {
      if (event.key === "Enter") {
        event.preventDefault();
        queueText(nicknameInput.value);
      }
      return;
    }

    if (state.pane === "create") {
      if (event.key === "Enter") {
        event.preventDefault();
        queueText(roomNameInput.value);
      }
      return;
    }

    const keyMap = {
      ArrowUp: 1000,
      ArrowDown: 1001,
      ArrowLeft: 1002,
      ArrowRight: 1003,
      Escape: 27,
      " ": 32,
      Enter: 10
    };

    if (Object.prototype.hasOwnProperty.call(keyMap, event.key)) {
      event.preventDefault();
      queueKey(keyMap[event.key]);
    }
  });

  renderMenuSelection();
  nicknameCurrent.textContent = `Current Nickname : [ ${state.nickname} ]`;

  return {
    pauseGame,
    showMenu
  };
}








