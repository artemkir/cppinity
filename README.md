# Compact C++ WASM Game Engine

A lightweight, compact game engine written in C++ that compiles to WebAssembly (WASM) for web-based games and playable ads. The idea is to use a similar code structure as Unity has, to ease porting from Unity to lightweight WASM code. This engine leverages libraries like Sokol for graphics rendering and is designed for simplicity and performance in browser environments. It is currently in its **early stages of development**, with core features implemented but many advanced capabilities still in progress.

## Features

### Implemented Core Features
- **Scene Management**: Inspired by Unity's ECS style, organize and manage game scenes with game objects, including Scene, GameObject, and Components for structured game logic.
- **Game Objects and Basic Components**: Entity-Component-System (ECS)-like architecture for flexible game logic, including transforms, colliders, input handlers, animations, and renderers (e.g., sprites and rectangles).
- **Resource Management**: Basic asynchronous resource loading (only textures for now) with fetch integration.
- **Simple UI Canvas System**: Added Canvas component for handling relative sizing, positioning, and anchoring (TopLeft or Center) with virtual resolutions. Supports aspect ratio maintenance for square or proportional canvases.
- **Rendering**: 2D rendering support using shaders, materials, and textures. Includes unlit color and texture shaders.
- **Input Handling**: Basic keyboard input support.
- **Example Game**: A simple Snake game demonstrating core features.
- **Testing**: Integrated Catch2 tests for some components and scene management logic.

### In Progress / Planned Features
- Basically everything!
- Make it more compact (now compiled Snake game is something around 0.5Mb)
- Support for 3D models and rendering.
- Audio playback and management.
- Advanced physics and collision detection.
- More tests.
- More examples and demos.

Note: The engine is not yet production-ready and is best suited for prototyping or small projects.

### Component Lifecycle Comparison with Unity
- The engine's BaseComponent provides lifecycle methods inspired by Unity to facilitate porting:
- In this engine, Awake() is called when the object is added to the scene, similar to Unity's Awake() which runs when the script instance is loaded.
- Start() is called at the first scene update, akin to Unity's Start() which executes before the first frame update.
- Update(float deltaTime) is invoked every frame, matching Unity's Update() for per-frame logic.
- OnCollide(GameObject* other) triggers on collision, comparable to Unity's OnCollisionEnter() or similar collision callbacks.
- SetActive(bool) toggles component activity, with OnActive(bool) for custom activation logic, resembling Unity's enabled property and OnEnable()/OnDisable().

### Prerequisites
- **Emscripten SDK**: Required for compiling C++ to WASM. Install from [emscripten.org](https://emscripten.org/docs/getting_started/downloads.html).
- **C++ Compiler**: Clang (included with Emscripten), GCC or MSVC.
- **Web Server**: A simple HTTP server (e.g., Python's `http.server`) to serve the WASM files, as browsers require this for security reasons.
- **Dependencies**: The engine uses external libraries like Sokol (included in the source). No additional installations needed beyond Emscripten.

### License
This project is licensed under the MIT License - see the LICENSE file for details.

### Acknowledgments
- Built with [Sokol](https://github.com/floooh/sokol) cross platform libraries.
