(() => {
    // Emscripten's generated glue script reads a global `Module` object for its config.
    // TODO: window.Module = { canvas: ..., ... }

    // TODO: load the emscripten output (the generated .js, which then loads the .wasm)
})();
