# ihtier blog

[Русский](README.ru.md)

Static blog on Astro + GitHub Pages with interactive GLSL shaders, C++ WASM apps and search.

## Features

- Markdown/MDX articles with images, video, syntax highlighting
- Interactive GLSL shaders (edit code → live result)
- C++ apps via WebAssembly (ImGui + SDL2 + Emscripten)
- Site search (Pagefind)
- Multilingual (EN / RU)

## Quick start

```bash
npm install
npm run dev
```

Site available at `http://localhost:4321/`

## Build

```bash
npm run build    # Full build (WASM + Astro + Pagefind)
npm run preview  # Preview built site
```

### Build flags

```bash
# Include demo article
INCLUDE_DEMO=true npm run build
```

### WASM apps

C++ → WASM compilation requires [Emscripten](https://emscripten.org/docs/getting_started):

```bash
# Install emsdk
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk && ./emsdk install latest && ./emsdk activate latest
source ./emsdk_env.sh

# Build WASM apps
npm run build:wasm
```

If `emcc` is not found, WASM build is skipped without error. In CI (GitHub Actions) Emscripten is installed automatically.

## Project structure

```
src/
├── pages/
│   ├── index.astro              # Redirect → /en/
│   ├── en/                      # English pages
│   │   ├── index.astro
│   │   └── articles/
│   │       └── demo.mdx
│   └── ru/                      # Russian pages
│       ├── index.astro
│       └── articles/
│           └── demo.mdx
├── layouts/
│   └── Base.astro               # Base layout (header, search, lang switch)
├── components/
│   ├── ShaderEditor.astro       # Interactive GLSL editor
│   ├── VideoEmbed.astro         # YouTube embed
│   └── WasmApp.astro            # WASM app embed (iframe)
├── styles/
│   └── global.css               # Global styles (dark theme)
└── i18n.ts                      # UI translations
wasm-apps/
├── build.sh                     # Build script for all WASM apps
└── imgui-demo/
    ├── main.cpp                 # ImGui demo app
    └── shell.html               # Emscripten HTML shell
public/wasm/                     # Compiled WASM files (generated)
scripts/prebuild.mjs             # INCLUDE_DEMO flag
.github/workflows/deploy.yml    # Auto-deploy to GitHub Pages
```

## Adding articles

### 1. Create file

Articles are `.mdx` files in `src/pages/{lang}/articles/`:

```bash
src/pages/en/articles/my-article.mdx
src/pages/ru/articles/my-article.mdx
```

### 2. Frontmatter

```mdx
---
layout: ../../../layouts/Base.astro
title: "Article title"
---
```

### 3. Content

Standard Markdown + components:

```mdx
import ShaderEditor from "../../../components/ShaderEditor.astro";
import VideoEmbed from "../../../components/VideoEmbed.astro";
import WasmApp from "../../../components/WasmApp.astro";

<div class="article-header">
  <h1>My article</h1>
  <div class="meta">2026-03-06</div>
</div>

<div class="article-content">

## Text

Text with **formatting** and `code`.

## Image

![description](/images/my-image.png)

## Video

<VideoEmbed src="https://www.youtube.com/watch?v=..." title="Title" />

## Shader

<ShaderEditor title="My shader" fragment={`precision mediump float;
uniform float u_time;
uniform vec2 u_resolution;
void main() {
    vec2 st = gl_FragCoord.xy / u_resolution;
    gl_FragColor = vec4(st, 0.5 + 0.5 * sin(u_time), 1.0);
}`} />

## WASM app

<WasmApp src="/wasm/my-app/index.html" title="My app" height="450px" />

</div>
```

### 4. Add to index

Edit `src/pages/en/index.astro` (and `ru/index.astro`):

```astro
<li>
  <a href="articles/my-article/">
    <h2>My article</h2>
    <span class="date">2026-03-06</span>
    <p class="summary">Article description.</p>
  </a>
</li>
```

## Adding WASM apps

1. Create `wasm-apps/my-app/` with `main.cpp`
2. Add compilation to `wasm-apps/build.sh`
3. Output goes to `public/wasm/my-app/`
4. Embed in article: `<WasmApp src="/wasm/my-app/index.html" />`

## Deploy

1. Create a GitHub repository
2. In Settings → Pages → Source select "GitHub Actions"
3. Update `site` in `astro.config.mjs` to your URL
4. Push to `main` — deploy happens automatically

## Adding a new language

1. Add locale to `astro.config.mjs` → `i18n.locales`
2. Add translations to `src/i18n.ts`
3. Create `src/pages/{lang}/` with index and articles
