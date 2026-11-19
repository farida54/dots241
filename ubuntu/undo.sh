#!/bin/sh
# Restore the normal Ubuntu login screen and desktop.
#     sudo sh ubuntu/undo.sh && sudo reboot
set -e

if [ "$(id -u)" -ne 0 ]; then
    echo "Run this with sudo." >&2
    exit 1
fi

rm -f /etc/systemd/system/getty@tty1.service.d/dots.conf
rmdir /etc/systemd/system/getty@tty1.service.d 2>/dev/null || true
systemctl daemon-reload
systemctl set-default graphical.target

echo "Restored. Reboot to get the desktop login back:  sudo reboot"
