#!/usr/bin/env bash
set -euo pipefail

# HappyBird Linux Uninstaller

PREFIX="${PREFIX:-/usr/local}"

echo "Uninstalling HappyBird..."

sudo rm -f  "$PREFIX/bin/happybird"
sudo rm -rf "$PREFIX/libexec/happybird"
sudo rm -rf "$PREFIX/share/happybird"
sudo rm -f  "$PREFIX/share/applications/happybird.desktop"
sudo rm -f  "$PREFIX/share/icons/hicolor/256x256/apps/happybird.png"

if command -v update-desktop-database >/dev/null 2>&1; then
    sudo update-desktop-database "$PREFIX/share/applications" 2>/dev/null || true
fi

echo "HappyBird has been uninstalled."
