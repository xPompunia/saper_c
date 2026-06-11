# saper_c

Minesweeper clone written in C with SDL2 — 16×16 board with 40 mines, flagging,
chord-reveal on numbers, a timer, and win/loss stats appended to
`game_stats.txt`.

## Building

The game was developed with Visual Studio on Windows (it uses `fopen_s`) and
links against SDL2. Add the sources to an SDL2-configured project and build
`main.c`; the sprite sheets live in `img/`.
