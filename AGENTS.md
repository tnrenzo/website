# AGENTS.md

## How to help on this repo

- The owner is **learning** and writes the code themselves. Don't produce full implementations unprompted.
  Give skeletons, explanations, or the specific piece they ask for.
- They're not a web-frontend person, so explain web-specific details when they come up.

## Repo overview

- Personal website. `main.go` is a tiny Go static file server for `public/`.
- Deployed via `Dockerfile` / `compose.yml`, which copies `public/` as is. There is no frontend build step.
- Frontend style: plain HTML, vanilla JS in an IIFE, one shared `public/style.css`. No frameworks or npm.

## `public/magnetic-field/`

Goal: a 3D visualisation of Uranus' and Neptune's magnetic fields with accurate position, rotation,
rotation speed and axis/dipole tilt. The camera transitions smoothly between the planets.

- `native/main.c`: **all** of the simulation and rendering, in C with raylib. Compiled to wasm with emscripten.
- `script.js`: only loads the emscripten output.
- `index.html`: hosts the canvas (id `canvas`, which raylib's web backend expects).

Notes:
- Planet positions should come from **JPL Horizons** (`https://ssd.jpl.nasa.gov/api/horizons.api`). SIMBAD doesn't cover
  Solar System objects. Browsers will likely need a proxy route in `main.go` (CORS).
- A raylib checkout exists at `~/dev/planet-magn/raylib` (6.5-dev) and emsdk at `~/emsdk`.
