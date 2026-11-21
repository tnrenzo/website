# AGENTS.md

## How to help on this repo

- The owner is **learning** and writes the code themselves. Don't produce full implementations unprompted.
  Give skeletons, explanations, or the specific piece they ask for.
- They're not a web-frontend person, so explain web-specific details when they come up.

## Repo overview

- Personal website. `main.go` is a tiny Go static file server for `public/`.
- Deployed via `Dockerfile` / `compose.yml`. The image copies `public/` as is, plus one build step:
  `public/magnetic-field/native/main.c` is compiled to wasm with emscripten, and the output
  (`magnetic-field.js` / `.wasm`) is copied into `public/magnetic-field/`. Nothing else is built.
- Frontend style: plain HTML, vanilla JS in an IIFE, a shared `public/style.css` (pages may add their
  own `style.css` with local overrides). No frameworks or npm.

## `public/magnetic-field/`

Goal: a 3D visualisation of Uranus' and Neptune's magnetic fields with accurate position, rotation,
rotation speed and axis/dipole tilt. The camera transitions smoothly between the planets.

- `native/main.c`: **all** of the simulation and rendering, in C with raylib. Compiled to wasm with emscripten.
- `native/Makefile`: native desktop build against the system raylib (not used for the web build).
- `index.html`: hosts the canvas (id `canvas`, which raylib's web backend expects), sets up `Module`
  inline and loads `magnetic-field.js`.

Notes:
- Planet positions should come from **JPL Horizons** (`https://ssd.jpl.nasa.gov/api/horizons.api`). SIMBAD doesn't cover
  Solar System objects. Browsers will likely need a proxy route in `main.go` (CORS).
- Local web build uses emsdk at `~/emsdk` and the raylib checkout at `./raylib` (gitignored). Any recent
  raylib commit works; the Dockerfile pins one via `RAYLIB_COMMIT`. Mirror the Dockerfile's `emcc` flags:
      source ~/emsdk/emsdk_env.sh
      make -C raylib/src PLATFORM=PLATFORM_WEB
      emcc public/magnetic-field/native/main.c -Os -DPLATFORM_WEB -I raylib/src \
          raylib/src/libraylib.web.a -s USE_GLFW=3 -o public/magnetic-field/magnetic-field.js
  then `go run .` and open `/magnetic-field/`.
