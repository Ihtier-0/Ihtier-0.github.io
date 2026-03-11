import { defineCollection, z } from "astro:content";

const postSchema = z.object({
  title: z.string(),
  date: z.union([z.string(), z.date()]).transform((d) =>
    d instanceof Date ? d.toISOString().slice(0, 10) : d
  ).optional(),
  summary: z.string().optional(),
});

export const collections = {
  articles: defineCollection({ type: "content", schema: postSchema }),
  etudes: defineCollection({ type: "content", schema: postSchema }),
  portfolio: defineCollection({ type: "content", schema: postSchema }),
};
