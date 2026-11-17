(() => {
    const scrambles = document.querySelectorAll('.scramble > *')

    scrambles.forEach(el => {
        el.style.left = (Math.random() * (window.innerWidth - el.offsetWidth)) + 'px'
        el.style.top = (Math.random() * (window.innerHeight - el.offsetHeight)) + 'px'
    })
})();