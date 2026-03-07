# ihtier blog

[English](README.md)

Статический блог на Astro + GitHub Pages с интерактивными GLSL-шейдерами, C++ WASM-приложениями и поиском.

## Возможности

- Статьи на Markdown/MDX с картинками, видео, подсветкой кода
- Интерактивные GLSL-шейдеры (редактируй код → результат в реальном времени)
- C++ приложения через WebAssembly (ImGui + SDL2 + Emscripten)
- Поиск по сайту (Pagefind)
- Мультиязычность (EN / RU)

## Быстрый старт

```bash
npm install
npm run dev
```

Сайт доступен на `http://localhost:4321/`

## Сборка

```bash
npm run build    # Полная сборка (WASM + Astro + Pagefind)
npm run preview  # Превью собранного сайта
```

### Флаги сборки

```bash
# Включить демо-статью в сборку
INCLUDE_DEMO=true npm run build
```

### WASM-приложения

Для компиляции C++ → WASM нужен [Emscripten](https://emscripten.org/docs/getting_started):

```bash
# Установка emsdk
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk && ./emsdk install latest && ./emsdk activate latest
source ./emsdk_env.sh

# Сборка WASM-приложений
npm run build:wasm
```

Если `emcc` не найден, WASM-сборка пропускается без ошибки. В CI (GitHub Actions) Emscripten устанавливается автоматически.

## Структура проекта

```
src/
├── pages/
│   ├── index.astro              # Редирект → /en/
│   ├── en/                      # English pages
│   │   ├── index.astro
│   │   └── articles/
│   │       └── demo.mdx
│   └── ru/                      # Русские страницы
│       ├── index.astro
│       └── articles/
│           └── demo.mdx
├── layouts/
│   └── Base.astro               # Базовый layout (шапка, поиск, язык)
├── components/
│   ├── ShaderEditor.astro       # Интерактивный GLSL-редактор
│   ├── VideoEmbed.astro         # YouTube embed
│   └── WasmApp.astro            # Встройка WASM-приложений (iframe)
├── styles/
│   └── global.css               # Глобальные стили (тёмная тема)
└── i18n.ts                      # Переводы UI
wasm-apps/
├── build.sh                     # Скрипт сборки всех WASM-приложений
└── imgui-demo/
    ├── main.cpp                 # ImGui демо-приложение
    └── shell.html               # HTML-шелл для Emscripten
public/wasm/                     # Скомпилированные WASM-файлы (генерируются)
scripts/prebuild.mjs             # Флаг INCLUDE_DEMO
.github/workflows/deploy.yml    # Автодеплой на GitHub Pages
```

## Добавление статей

### 1. Создай файл

Статьи — это `.mdx` файлы в `src/pages/{lang}/articles/`:

```bash
src/pages/en/articles/my-article.mdx
src/pages/ru/articles/my-article.mdx
```

### 2. Заполни frontmatter

```mdx
---
layout: ../../../layouts/Base.astro
title: "Название статьи"
---
```

### 3. Пиши контент

Обычный Markdown + компоненты:

```mdx
import ShaderEditor from "../../../components/ShaderEditor.astro";
import VideoEmbed from "../../../components/VideoEmbed.astro";
import WasmApp from "../../../components/WasmApp.astro";

<div class="article-header">
  <h1>Моя статья</h1>
  <div class="meta">2026-03-06</div>
</div>

<div class="article-content">

## Текст

Текст с **форматированием** и `кодом`.

## Картинка

![описание](/images/my-image.png)

## Видео

<VideoEmbed src="https://www.youtube.com/watch?v=..." title="Название" />

## Шейдер

<ShaderEditor title="Мой шейдер" fragment={`precision mediump float;
uniform float u_time;
uniform vec2 u_resolution;
void main() {
    vec2 st = gl_FragCoord.xy / u_resolution;
    gl_FragColor = vec4(st, 0.5 + 0.5 * sin(u_time), 1.0);
}`} />

## WASM-приложение

<WasmApp src="/wasm/my-app/index.html" title="Моё приложение" height="450px" />

</div>
```

### 4. Добавь на главную

Отредактируй `src/pages/en/index.astro` (и `ru/index.astro`):

```astro
<li>
  <a href="articles/my-article/">
    <h2>Моя статья</h2>
    <span class="date">2026-03-06</span>
    <p class="summary">Описание статьи.</p>
  </a>
</li>
```

## Добавление WASM-приложений

1. Создай папку `wasm-apps/my-app/` с `main.cpp`
2. Добавь компиляцию в `wasm-apps/build.sh`
3. Вывод идёт в `public/wasm/my-app/`
4. Встрой в статью: `<WasmApp src="/wasm/my-app/index.html" />`

## Деплой

1. Создай репозиторий на GitHub
2. В Settings → Pages → Source выбери "GitHub Actions"
3. Обнови `site` в `astro.config.mjs` на свой URL
4. Пуш в `main` — деплой произойдёт автоматически

## Добавление нового языка

1. Добавь локаль в `astro.config.mjs` → `i18n.locales`
2. Добавь переводы в `src/i18n.ts`
3. Создай `src/pages/{lang}/` с index и статьями
