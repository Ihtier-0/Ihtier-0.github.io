#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/.."
OUT_DIR="$ROOT_DIR/public/wasm/imgui-demo"
IMGUI_DIR="$SCRIPT_DIR/imgui"
IMGUI_VERSION="v1.91.8"
EMSDK_DIR="$ROOT_DIR/emsdk"
EMSDK_VERSION="5.0.2"

# ── Step 0: Ensure Python is available (Windows Store alias workaround) ─
if [ -z "$EMSDK_PYTHON" ] && ! python3 --version &> /dev/null; then
    for p in "$LOCALAPPDATA/Programs/Python"/Python*/python.exe \
             /c/Users/*/AppData/Local/Programs/Python/Python*/python.exe \
             /c/Python*/python.exe; do
        if [ -x "$p" ]; then
            export EMSDK_PYTHON="$p"
            export PATH="$(dirname "$p"):$PATH"
            break
        fi
    done
fi

# ── Step 1: Ensure emcc is available ──────────────────────────────────
EMCC="$(command -v emcc 2>/dev/null || true)"

if [ -z "$EMCC" ]; then
    echo "[wasm] emcc not found on PATH, bootstrapping from emsdk submodule..."

    # Ensure the submodule is initialized
    if [ ! -f "$EMSDK_DIR/emsdk" ] && [ ! -f "$EMSDK_DIR/emsdk.bat" ]; then
        echo "[wasm] Initializing emsdk submodule..."
        git -C "$ROOT_DIR" submodule update --init --depth 1 emsdk
    fi

    # Install the pinned version if not already installed
    if [ ! -d "$EMSDK_DIR/upstream" ]; then
        echo "[wasm] Installing Emscripten $EMSDK_VERSION (first time, may take a few minutes)..."
        "$EMSDK_DIR/emsdk" install "$EMSDK_VERSION"
    fi

    "$EMSDK_DIR/emsdk" activate "$EMSDK_VERSION"
    source "$EMSDK_DIR/emsdk_env.sh" 2>/dev/null || true

    # Find emcc — on Windows it's emcc.bat (not found by `command -v`)
    EMCC="$(command -v emcc 2>/dev/null || true)"
    if [ -z "$EMCC" ] && [ -f "$EMSDK_DIR/upstream/emscripten/emcc.bat" ]; then
        EMCC="$EMSDK_DIR/upstream/emscripten/emcc.bat"
    fi

    if [ -z "$EMCC" ]; then
        echo "[wasm] ERROR: emcc not found after emsdk setup"
        exit 1
    fi

    echo "[wasm] emcc ready: $("$EMCC" --version 2>&1 | head -1)"
fi

# ── Step 2: Download ImGui if not present ─────────────────────────────
if [ ! -d "$IMGUI_DIR" ]; then
    echo "[wasm] Downloading imgui $IMGUI_VERSION..."
    git clone --depth 1 --branch "$IMGUI_VERSION" \
        https://github.com/ocornut/imgui.git "$IMGUI_DIR"
fi

# ── Step 3: Compile ───────────────────────────────────────────────────
mkdir -p "$OUT_DIR"

echo "[wasm] Compiling imgui-demo..."
"$EMCC" \
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
