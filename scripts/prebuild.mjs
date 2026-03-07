import { rmSync, existsSync } from "fs";
import { resolve, dirname } from "path";
import { fileURLToPath } from "url";

const root = resolve(dirname(fileURLToPath(import.meta.url)), "..");

if (process.env.INCLUDE_DEMO !== "true") {
  const demos = [
    "src/pages/ru/articles/demo.mdx",
    "src/pages/en/articles/demo.mdx",
  ];
  for (const f of demos) {
    const full = resolve(root, f);
    if (existsSync(full)) {
      rmSync(full);
      console.log(`[prebuild] Excluded demo article: ${f}`);
    }
  }
} else {
  console.log("[prebuild] Demo articles included (INCLUDE_DEMO=true)");
}
