# DontCrashEngine

A custom 2D game engine written in C++20, built on top of SDL2, Box2D, and Lua. Ships with a sandbox/tower-defense demo game (`DontCrashGame`) that runs on a tile map.

---

## Requirements

- **Visual Studio 2022**
- **C++20**
- Windows
- All third-party libraries are vendored under `DontCrashEngine/libs/` — no package manager needed

---

## Building

1. Open `DontCrashEngine.sln` in Visual Studio 2022.
2. Select solution build **Debug** or **Release**, **x86** or **x64**.
3. Build the solution (`Ctrl+Shift+B`). Project dependencies are declared in the `.sln` so they build in the correct order.
4. GluaGen runs automatically as a pre-build step for `DontCrashEngine` and `DontCrashGame`. If you modify any `LUA_*` macros in a header, the matching `.gen` file is regenerated before the rest of the build proceeds.
5. Run `ResourcePacker` once to produce the asset pack before launching `DontCrashGame`.

---

## Architecture Overview

```
ApplicationLayer  (singleton — GetInstance())
├── WindowManager        (windowing + input)
├── EngineGraphics       (renderer)
├── DC_Audio             (audio)
├── Logger
└── GameStateMachine     (the active game)
    ├── ObjectFactory
    ├── ScriptingManager (Lua state)
    ├── DC_PhysicsManager
    ├── EventManager
    ├── ProcessManager
    ├── ResourceFile
    └── GameState        (current state: MainMenu / GamePlay / Pause / End)
        ├── GameScene
        │   ├── GameObject[]  (unordered_map<id, shared_ptr<GameObject>>)
        │   └── DC_View[]     (unordered_map<id, unique_ptr<DC_View>>)
        ├── ViewFactory
        └── SceneFactory
```

Each `GameObject` is a bag of `DC_ObjectComponent`s looked up by integer ID. Data-driven construction happens through XML files read by `ObjectFactory`.

---

