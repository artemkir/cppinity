:: Install and activate emsdk first
:: winget install Ninja-build.Ninja
:: git clone https://github.com/emscripten-core/emsdk
:: cd emsdk
:: emsdk install latest
:: emsdk activate latest

:: Change Snake to other sample name to build

emcmake cmake -S . -B build-wasm -DCMAKE_BUILD_TYPE=Release
cmake --build build-wasm --target Snake -j

:: emrun --port 8080 build-wasm/bin