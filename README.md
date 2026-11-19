# dots241 — Dots and Boxes (Sprint 1)

Two-player console Dots and Boxes in C: a 4x5 grid of boxes (5 rows x 6 columns of dots).
Players A and B take turns drawing a line between two neighbouring dots. A player who
completes a box gets it (their letter appears inside it) and plays again. When all
20 boxes are claimed, the player with more boxes wins.

## Files

| File | Purpose |
|------|---------|
| `dots.h` | Board size constants |
| `board.h` / `board.c` | Printing the board, drawing lines, detecting and claiming boxes |
| `main.c` | Game loop: input, turns, scores, winner |
| `Makefile` | Build, debugging and install targets |
| `tests/` | Scripted inputs: a full game, and an input that ends early |
| `debugging/` | GDB script and saved Valgrind/GDB logs |
| `alpine/` | Scripts that make the Alpine VM boot straight into the game |

## Build and play

```sh
make
./dots
```

Enter a move as four numbers: row and column of the first dot, then of the second dot.
Example: `0 0 0 1` draws the top-left horizontal line. Invalid input, lines that are not
between neighbouring dots, and lines that are already drawn are rejected, and the same
player tries again.

## Debugging (Valgrind and GDB)

```sh
make valgrind      # memcheck on a full game and on early end-of-input
make gdb-session   # scripted GDB session, output saved to debugging/gdb_session.txt
```

**Valgrind** (`--leak-check=full --show-leak-kinds=all --track-origins=yes`) runs two cases:
- `tests/full_game.txt`: bad input (text, a diagonal, off-board dots, a repeated line),
  then every one of the 49 lines until the game ends.
- `tests/early_eof.txt`: input stops in the middle of the game.

Both report `0 errors` and `All heap blocks were freed -- no leaks are possible`
(see `debugging/valgrind_*.txt`).

**GDB** (`debugging/session.gdb`) runs the full game and uses:
1. a *conditional breakpoint* on `draw_line` that stops only on the diagonal move
   `0 0 1 1`, then `finish` to show it returns `0` (rejected);
2. a breakpoint where a box is claimed in `try_claim`: `info args`, `backtrace`
   (main → check_boxes → try_claim) and printing the box's 4 sides;
3. a breakpoint at the end of the game, printing `lines_drawn` (49), both scores
   (they add up to 20) and the full `box` array.

To debug by hand: `gdb ./dots`, then for example `break check_boxes`, `run`,
`next`, `print box`, `watch scoreA`.

Bug found while debugging: input was read with `scanf`, so extra text after the four
numbers (`0 0 0 1 extra`) stayed in the input and showed up as an error on the next
turn. Fixed by reading one full line with `fgets` and parsing it with `sscanf`.

## Alpine Linux VM: boot straight into the game

1. Download the **Alpine "Virtual" x86_64 ISO** from https://alpinelinux.org/downloads/.
2. Create a VM (VirtualBox, VMware, QEMU or UTM): 512 MB RAM, 2 GB disk, boot from the ISO.
3. Log in as `root` (no password) and run `setup-alpine`. Pick your keyboard, network
   (`eth0`, `dhcp`), a root password, and at the end the disk `sda` with mode **`sys`**.
4. Reboot, remove the ISO, log in as root.
5. Get the code and run the setup script:
   ```sh
   apk add git
   git clone https://github.com/farida54/<repo-name>.git dots241
   cd dots241
   sh alpine/setup.sh
   reboot
   ```

After the reboot, the game appears on screen with no login prompt.

How it works: Alpine's init (busybox) reads `/etc/inittab`. The line
`tty1::respawn:/sbin/getty 38400 tty1` normally shows the login prompt on the first
console. `setup.sh` replaces it with `tty1::respawn:/usr/local/bin/dots-launch`, so init
starts the game on tty1 instead, and `respawn` restarts it if it ever exits.
`dots-launch` runs the game in a loop ("Press Enter to play again").

- A normal login is still available on the second console: **Alt+F2**.
- To undo: `cp /etc/inittab.orig /etc/inittab && reboot`.

## Ubuntu VM: boot straight into the game

Same idea as Alpine, using systemd. Run from the repository folder:

```sh
make
sudo sh ubuntu/setup.sh
sudo reboot
```

`setup.sh` installs the game, adds a drop-in file
`/etc/systemd/system/getty@tty1.service.d/dots.conf` so the tty1 service runs
`dots-launch` instead of the login prompt, and sets the default boot target to
`multi-user.target` (text console, no desktop).

- Normal login: **Ctrl+Alt+F2**. Start the desktop once: `sudo systemctl start lightdm`.
- Undo everything: `sudo sh ubuntu/undo.sh && sudo reboot`.
