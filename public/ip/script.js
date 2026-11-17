(() => {
    const get = (el, url) => fetch(url)
        .then(r => r.text())
        .then(t => el.textContent = t, () => el.textContent = "none");
    get(v4, "https://v4.zzzzzz.ing");
    get(v6, "https://v6.zzzzzz.ing");
    ua.textContent = navigator.userAgent;
})();