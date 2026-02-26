#!/usr/bin/env bash
set -euo pipefail

# HappyBird Linux Installer
# Builds from source and installs to /usr/local

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

PREFIX="${PREFIX:-/usr/local}"
BIN_DIR="$PREFIX/bin"
LIBEXEC_DIR="$PREFIX/libexec/happybird"
SHARE_DIR="$PREFIX/share/happybird"
DESKTOP_DIR="$PREFIX/share/applications"
ICON_DIR="$PREFIX/share/icons/hicolor/256x256/apps"
BUILD_DIR="$PROJECT_ROOT/build-linux"

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

info()  { echo -e "${GREEN}[INFO]${NC} $*"; }
warn()  { echo -e "${YELLOW}[WARN]${NC} $*"; }
error() { echo -e "${RED}[ERROR]${NC} $*"; exit 1; }

# ------------------------------------------------------------------
# Step 1: Check/install build dependencies
# ------------------------------------------------------------------
check_dependencies() {
    info "Checking build dependencies..."

    local missing=()

    command -v cmake >/dev/null 2>&1 || missing+=(cmake)
    command -v ninja >/dev/null 2>&1 || missing+=(ninja-build)
    command -v g++   >/dev/null 2>&1 || missing+=(g++)
    command -v git   >/dev/null 2>&1 || missing+=(git)

    if [ ${#missing[@]} -gt 0 ]; then
        info "Installing missing packages: ${missing[*]}"
        if command -v apt-get >/dev/null 2>&1; then
            sudo apt-get update
            sudo apt-get install -y "${missing[@]}"
        elif command -v dnf >/dev/null 2>&1; then
            sudo dnf install -y "${missing[@]}"
        elif command -v pacman >/dev/null 2>&1; then
            local arch_pkgs=()
            for pkg in "${missing[@]}"; do
                [[ "$pkg" == "ninja-build" ]] && arch_pkgs+=(ninja) || arch_pkgs+=("$pkg")
            done
            sudo pacman -S --noconfirm "${arch_pkgs[@]}"
        else
            error "Cannot auto-install packages. Please install manually: ${missing[*]}"
        fi
    fi

    # Install SFML build dependencies (X11, OpenGL, audio libs)
    if command -v apt-get >/dev/null 2>&1; then
        info "Installing SFML build dependencies..."
        sudo apt-get install -y \
            libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
            libudev-dev libgl1-mesa-dev \
            libopenal-dev libvorbis-dev libflac-dev \
            libfreetype-dev
    elif command -v dnf >/dev/null 2>&1; then
        sudo dnf install -y \
            libX11-devel libXrandr-devel libXcursor-devel libXi-devel \
            mesa-libGL-devel systemd-devel \
            openal-soft-devel libvorbis-devel flac-devel \
            freetype-devel
    elif command -v pacman >/dev/null 2>&1; then
        sudo pacman -S --noconfirm \
            libx11 libxrandr libxcursor libxi \
            mesa systemd \
            openal libvorbis flac \
            freetype2
    fi
}

# ------------------------------------------------------------------
# Step 2: Build or find SFML 3
# ------------------------------------------------------------------
ensure_sfml3() {
    info "Checking for SFML 3..."

    # Check if SFML 3 is already available via pkg-config
    if pkg-config --atleast-version=3.0 sfml-all 2>/dev/null; then
        info "SFML >= 3.0 found on system."
        return 0
    fi

    # Try a test CMake configure to see if find_package(SFML 3.0) would work
    local test_dir="/tmp/sfml3-test"
    mkdir -p "$test_dir"
    cat > "$test_dir/CMakeLists.txt" << 'EOF'
cmake_minimum_required(VERSION 3.21)
project(sfml_test)
find_package(SFML 3.0 COMPONENTS Graphics Audio)
if(SFML_FOUND)
    message(STATUS "SFML_FOUND=YES")
endif()
EOF

    if cmake -S "$test_dir" -B "$test_dir/build" 2>&1 | grep -q "SFML_FOUND=YES"; then
        info "SFML 3 found via CMake."
        rm -rf "$test_dir"
        return 0
    fi
    rm -rf "$test_dir"

    # Build SFML 3 from source
    warn "SFML 3 not found. Building from source..."
    local sfml_src_dir="/tmp/sfml3-src"
    local sfml_build_dir="/tmp/sfml3-build"

    if [ ! -d "$sfml_src_dir" ]; then
        git clone --depth 1 --branch 3.0.0 https://github.com/SFML/SFML.git "$sfml_src_dir"
    fi

    cmake -S "$sfml_src_dir" -B "$sfml_build_dir" \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX="/usr/local" \
        -DBUILD_SHARED_LIBS=ON

    cmake --build "$sfml_build_dir" --config Release
    sudo cmake --install "$sfml_build_dir"
    sudo ldconfig

    info "SFML 3 installed to /usr/local"
}

# ------------------------------------------------------------------
# Step 3: Build HappyBird
# ------------------------------------------------------------------
build_happybird() {
    info "Building HappyBird..."

    cmake -S "$PROJECT_ROOT" -B "$BUILD_DIR" \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release

    cmake --build "$BUILD_DIR" --config Release

    info "Build complete."
}

# ------------------------------------------------------------------
# Step 4: Install
# ------------------------------------------------------------------
install_happybird() {
    info "Installing HappyBird to $PREFIX ..."

    sudo mkdir -p "$BIN_DIR" "$LIBEXEC_DIR" "$SHARE_DIR" "$DESKTOP_DIR" "$ICON_DIR"

    # Install binary
    sudo install -m 755 "$BUILD_DIR/HappyBird" "$LIBEXEC_DIR/happybird-bin"

    # Install assets
    sudo install -m 644 "$BUILD_DIR"/*.bmp "$SHARE_DIR/"
    sudo install -m 644 "$BUILD_DIR"/*.wav "$SHARE_DIR/"
    sudo install -m 644 "$BUILD_DIR"/*.jpg "$SHARE_DIR/" 2>/dev/null || true
    sudo install -m 644 "$BUILD_DIR/Bicubik.OTF" "$SHARE_DIR/"
    sudo install -m 644 "$BUILD_DIR/favicon.ico" "$SHARE_DIR/"

    # Create wrapper script that cd's to asset dir before running
    sudo tee "$BIN_DIR/happybird" > /dev/null << WRAPPER
#!/usr/bin/env bash
cd "$SHARE_DIR"
exec "$LIBEXEC_DIR/happybird-bin" "\$@"
WRAPPER
    sudo chmod 755 "$BIN_DIR/happybird"

    # Install desktop entry
    sudo tee "$DESKTOP_DIR/happybird.desktop" > /dev/null << DESKTOP
[Desktop Entry]
Type=Application
Name=HappyBird
Comment=A Flappy Bird-style game built with SFML
Exec=$BIN_DIR/happybird
Icon=happybird
Categories=Game;ArcadeGame;
Terminal=false
DESKTOP

    # Convert .ico to .png for the icon if ImageMagick is available
    if command -v convert >/dev/null 2>&1; then
        convert "$SHARE_DIR/favicon.ico" -resize 256x256 "/tmp/happybird.png"
        sudo install -m 644 "/tmp/happybird.png" "$ICON_DIR/happybird.png"
        rm -f /tmp/happybird.png
    else
        warn "ImageMagick not found. Desktop icon not installed."
        warn "Install imagemagick and re-run to get the application icon."
    fi

    # Update desktop database
    if command -v update-desktop-database >/dev/null 2>&1; then
        sudo update-desktop-database "$DESKTOP_DIR" 2>/dev/null || true
    fi

    info "Installation complete!"
    info "Run 'happybird' from a terminal or find it in your application launcher."
}

# ------------------------------------------------------------------
# Print uninstall instructions
# ------------------------------------------------------------------
print_uninstall_info() {
    echo ""
    info "To uninstall, run:"
    echo "  ./installer/uninstall.sh"
    echo ""
    echo "  Or manually:"
    echo "    sudo rm -f  $BIN_DIR/happybird"
    echo "    sudo rm -rf $LIBEXEC_DIR"
    echo "    sudo rm -rf $SHARE_DIR"
    echo "    sudo rm -f  $DESKTOP_DIR/happybird.desktop"
    echo "    sudo rm -f  $ICON_DIR/happybird.png"
    echo ""
}

# ------------------------------------------------------------------
# Main
# ------------------------------------------------------------------
main() {
    echo "========================================"
    echo "  HappyBird Linux Installer"
    echo "========================================"
    echo ""

    check_dependencies
    ensure_sfml3
    build_happybird
    install_happybird
    print_uninstall_info
}

main "$@"
