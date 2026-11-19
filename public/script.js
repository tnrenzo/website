(() => {
    const elements = [...document.querySelectorAll('.scramble > *')]
    const placed = []

    function scramble() {
        placed.length = 0

        elements
            .sort((a, b) => b.offsetWidth * b.offsetHeight - a.offsetWidth * a.offsetHeight)
            .forEach(el => {
                const gap = 12
                const positions = []

                for (let top = 0; top + el.offsetHeight <= innerHeight; top += gap) {
                    for (let left = 0; left + el.offsetWidth <= innerWidth; left += gap) {
                        positions.push([left, top])
                    }
                }

                positions.sort(() => Math.random() - 0.5)

                for (const [left, top] of positions) {
                    el.style.left = left + 'px'
                    el.style.top = top + 'px'
                    const rect = el.getBoundingClientRect()
                    if (placed.every(other =>
                        rect.right + gap <= other.left ||
                        rect.left >= other.right + gap ||
                        rect.bottom + gap <= other.top ||
                        rect.top >= other.bottom + gap
                    )) {
                        placed.push(rect)
                        break
                    }
                }
            })
    }

    window.addEventListener('resize', scramble)
    document.fonts?.ready.then(scramble)
    scramble()
})();