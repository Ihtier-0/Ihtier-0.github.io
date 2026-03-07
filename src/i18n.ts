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
    "home.title": "Статьи",
    "article.back": "← Назад",
  },
  en: {
    "site.title": "Ihtier-0 blog",
    "search.placeholder": "Search...",
    "search.empty": "Nothing found",
    "home.title": "Articles",
    "article.back": "← Back",
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
