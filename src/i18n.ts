export const languages = {
  ru: "Русский",
  en: "English",
} as const;

export type Lang = keyof typeof languages;

const ui = {
  ru: {
    "site.title": "Ihtier-0 blog",
    "search.placeholder": "Поиск...",
    "search.empty": "Ничего не найдено",
    "home.title": "Главная",
    "nav.home": "Главная",
    "nav.articles": "Статьи",
    "nav.etudes": "C++ Этюды",
    "nav.about": "Обо мне",
    "nav.blog": "Блог",
    "article.back": "← Назад",
    "tag.article": "Статья",
    "tag.etude": "C++ Этюд",
    "tag.portfolio": "Портфолио",
    "list.empty.posts": "Записей пока нет.",
    "list.empty.articles": "Статей пока нет.",
    "list.empty.etudes": "Этюдов пока нет.",
    "list.empty.portfolio": "Пока нет записей.",
    "page.articles": "Статьи",
    "page.etudes": "C++ Этюды",
    "page.portfolio": "Портфолио",
    "nav.portfolio": "Портфолио",
    "footer.built": "Сделано с",
  },
  en: {
    "site.title": "Ihtier-0 blog",
    "search.placeholder": "Search...",
    "search.empty": "Nothing found",
    "home.title": "Home",
    "nav.home": "Home",
    "nav.articles": "Articles",
    "nav.etudes": "C++ Etudes",
    "nav.about": "About",
    "nav.blog": "Blog",
    "article.back": "← Back",
    "tag.article": "Article",
    "tag.etude": "C++ Etude",
    "tag.portfolio": "Portfolio",
    "list.empty.posts": "No posts yet.",
    "list.empty.articles": "No articles yet.",
    "list.empty.etudes": "No etudes yet.",
    "list.empty.portfolio": "No entries yet.",
    "page.articles": "Articles",
    "page.etudes": "C++ Etudes",
    "page.portfolio": "Portfolio",
    "nav.portfolio": "Portfolio",
    "footer.built": "Built with",
  },
} as const;

export function t(lang: Lang, key: keyof (typeof ui)["ru"]): string {
  return ui[lang][key] ?? ui.ru[key];
}

export function getLangFromUrl(url: URL): Lang {
  const seg = url.pathname.split("/").filter(Boolean);
  if (seg[0] && seg[0] in languages) return seg[0] as Lang;
  return "ru";
}

export function getOtherLang(lang: Lang): Lang {
  return lang === "ru" ? "en" : "ru";
}
