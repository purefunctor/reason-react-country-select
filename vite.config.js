import { defineConfig } from "vite";
import { nodeResolve } from "@rollup/plugin-node-resolve";

export default defineConfig({
  base: "/reason-react-country-select/",
  plugins: [nodeResolve()],
});
