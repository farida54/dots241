#!/bin/sh
# Make this Ubuntu machine boot straight into Dots and Boxes on the text console,
# with no login and no desktop. Run from the root of the repository:
#     make
#     sudo sh ubuntu/setup.sh
#     sudo reboot
# Undo with:  sudo sh ubuntu/undo.sh
set -e

if [ "$(id -u)" -ne 0 ]; then
    echo "Run this with sudo." >&2
    exit 1
fi
if [ ! -x ./dots ]; then
    echo "Build first: run 'make' (without sudo), then run this again." >&2
    exit 1
fi

echo "[1/3] Installing the game to /usr/local/bin..."
install -m 755 dots /usr/local/bin/dots
install -m 755 alpine/dots-launch /usr/local/bin/dots-launch

echo "[2/3] Replacing the tty1 login prompt with the game..."
mkdir -p /etc/systemd/system/getty@tty1.service.d
cat > /etc/systemd/system/getty@tty1.service.d/dots.conf <<'CONF'
[Service]
ExecStart=
ExecStart=-/usr/local/bin/dots-launch
CONF
systemctl daemon-reload

echo "[3/3] Booting to the text console instead of the desktop..."
systemctl set-default multi-user.target

echo
echo "Done. Reboot to test:  sudo reboot"
echo "A normal login is still on Ctrl+Alt+F2."
echo "Desktop once, without undoing:  sudo systemctl start lightdm"
echo "Undo everything:  sudo sh ubuntu/undo.sh"
