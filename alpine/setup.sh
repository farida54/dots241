#!/bin/sh
# Run ONCE as root inside the Alpine VM, from the root of this repository:
#     sh alpine/setup.sh
# Builds the game, installs it, and replaces the tty1 login prompt with the game,
# so that booting the VM goes straight into Dots and Boxes (no username/password).
set -e

if [ "$(id -u)" -ne 0 ]; then
    echo "Run this as root." >&2
    exit 1
fi

echo "[1/4] Installing compiler and make..."
apk add --no-cache build-base

echo "[2/4] Building..."
make clean
make

echo "[3/4] Installing to /usr/local/bin..."
make install

echo "[4/4] Configuring /etc/inittab..."
[ -f /etc/inittab.orig ] || cp /etc/inittab /etc/inittab.orig
# tty1: start the game directly instead of getty/login.
sed -i 's|^tty1::respawn:.*|tty1::respawn:/usr/local/bin/dots-launch|' /etc/inittab
grep -q '^tty1::respawn:/usr/local/bin/dots-launch' /etc/inittab || {
    echo "Could not find a tty1 line in /etc/inittab; adding one."
    echo 'tty1::respawn:/usr/local/bin/dots-launch' >> /etc/inittab
}
# tty2 keeps a normal login (Alt+F2) for maintenance.
grep -q '^tty2::' /etc/inittab || echo 'tty2::respawn:/sbin/getty 38400 tty2' >> /etc/inittab

# Diskless/live installs only keep changes that are saved with lbu.
if command -v lbu >/dev/null 2>&1 && mount | grep -q ' on / type tmpfs'; then
    lbu include /usr/local/bin/dots /usr/local/bin/dots-launch
    lbu commit || true
fi

echo
echo "Done. tty1 now runs:"
grep '^tty1::' /etc/inittab
echo "Reboot to test:  reboot"
echo "Undo:  cp /etc/inittab.orig /etc/inittab && reboot"
