#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
OUT_DIR="$SCRIPT_DIR/../public/wasm/imgui-demo"
IMGUI_DIR="$SCRIPT_DIR/imgui"
IMGUI_VERSION="v1.91.8"

# Check if emcc is available
if ! command -v emcc &> /dev/null; then
    echo "[wasm] emcc not found — skipping WASM build"
    echo "[wasm] Install Emscripten: https://emscripten.org/docs/getting_started"
    exit 0
fi

# Download imgui if not present
if [ ! -d "$IMGUI_DIR" ]; then
    echo "[wasm] Downloading imgui $IMGUI_VERSION..."
    git clone --depth 1 --branch "$IMGUI_VERSION" https://github.com/ocornut/imgui.git "$IMGUI_DIR"
fi

mkdir -p "$OUT_DIR"

echo "[wasm] Compiling imgui-demo..."
emcc \
    "$SCRIPT_DIR/imgui-demo/main.cpp" \
    "$IMGUI_DIR/imgui.cpp" \
    "$IMGUI_DIR/imgui_draw.cpp" \
    "$IMGUI_DIR/imgui_tables.cpp" \
    "$IMGUI_DIR/imgui_widgets.cpp" \
    "$IMGUI_DIR/backends/imgui_impl_sdl2.cpp" \
    "$IMGUI_DIR/backends/imgui_impl_opengl3.cpp" \
    -I"$IMGUI_DIR" \
    -I"$IMGUI_DIR/backends" \
    -s USE_SDL=2 \
    -s FULL_ES3=1 \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s MIN_WEBGL_VERSION=2 \
    -s MAX_WEBGL_VERSION=2 \
    --shell-file "$SCRIPT_DIR/imgui-demo/shell.html" \
    -O2 \
    -o "$OUT_DIR/index.html"

echo "[wasm] Done → $OUT_DIR"
