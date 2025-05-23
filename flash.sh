#!/usr/bin/env bash
set -e

# Dual-rate LED blinker flash script
# Supports multiple flashing tools: st-link, OpenOCD, and J-Link

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
HEX_FILE="${1:-build-firmware/blinky.hex}"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_usage() {
    echo "Usage: $0 [HEX_FILE] [TOOL]"
    echo ""
    echo "Arguments:"
    echo "  HEX_FILE    Path to hex file (default: build-firmware/blinky.hex)"
    echo "  TOOL        Flashing tool: stlink, openocd, jlink (auto-detect if not specified)"
    echo ""
    echo "Examples:"
    echo "  $0                                    # Flash default hex with auto-detected tool"
    echo "  $0 build-firmware/blinky.hex         # Flash specific hex file"
    echo "  $0 build-firmware/blinky.hex stlink  # Force use of st-link"
    echo ""
    echo "Requirements:"
    echo "  - ST-Link: Install stlink package (st-flash command)"
    echo "  - OpenOCD: Install openocd package"
    echo "  - J-Link: Install J-Link software from SEGGER"
}

detect_tool() {
    if command -v st-flash &> /dev/null; then
        echo "stlink"
    elif command -v openocd &> /dev/null; then
        echo "openocd"
    elif command -v JLinkExe &> /dev/null; then
        echo "jlink"
    else
        echo "none"
    fi
}

flash_stlink() {
    local hex_file="$1"
    echo -e "${GREEN}Flashing with ST-Link...${NC}"
    st-flash --reset write "$hex_file" 0x8000000
}

flash_openocd() {
    local hex_file="$1"
    echo -e "${GREEN}Flashing with OpenOCD...${NC}"
    openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
        -c "program $hex_file verify reset exit"
}

flash_jlink() {
    local hex_file="$1"
    local temp_script=$(mktemp)
    
    echo -e "${GREEN}Flashing with J-Link...${NC}"
    
    # Create J-Link script
    cat > "$temp_script" << EOF
device STM32F103C8
si 1
speed 4000
loadfile $hex_file
r
h
exit
EOF
    
    JLinkExe -commanderscript "$temp_script"
    rm -f "$temp_script"
}

# Parse arguments
TOOL="${2:-$(detect_tool)}"

# Check if help requested
if [[ "$1" == "-h" ]] || [[ "$1" == "--help" ]]; then
    print_usage
    exit 0
fi

# Validate hex file
if [[ ! -f "$HEX_FILE" ]]; then
    echo -e "${RED}Error: Hex file not found: $HEX_FILE${NC}"
    echo "Did you build the firmware first?"
    echo "Try: cmake -B build-firmware -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc.cmake && cmake --build build-firmware"
    exit 1
fi

# Validate tool
if [[ "$TOOL" == "none" ]]; then
    echo -e "${RED}Error: No supported flashing tool found!${NC}"
    echo ""
    echo "Please install one of the following:"
    echo "  - ST-Link tools: sudo apt install stlink-tools"
    echo "  - OpenOCD: sudo apt install openocd"
    echo "  - J-Link: Download from https://www.segger.com/downloads/jlink/"
    exit 1
fi

echo -e "${YELLOW}Flashing STM32F103 Blue Pill...${NC}"
echo "Hex file: $HEX_FILE"
echo "Tool: $TOOL"
echo ""

# Flash based on selected tool
case "$TOOL" in
    "stlink")
        flash_stlink "$HEX_FILE"
        ;;
    "openocd")
        flash_openocd "$HEX_FILE"
        ;;
    "jlink")
        flash_jlink "$HEX_FILE"
        ;;
    *)
        echo -e "${RED}Error: Unsupported tool: $TOOL${NC}"
        echo "Supported tools: stlink, openocd, jlink"
        exit 1
        ;;
esac

if [[ $? -eq 0 ]]; then
    echo -e "${GREEN}✓ Flashing completed successfully!${NC}"
    echo ""
    echo "Your STM32 should now be running the dual-rate LED blinker:"
    echo "  - Red LED (PC13): Fast blink at 2.5 Hz (200ms toggle)"
    echo "  - Green LED (PB12): Slow blink at 0.5 Hz (1000ms toggle)"
else
    echo -e "${RED}✗ Flashing failed!${NC}"
    echo ""
    echo "Troubleshooting:"
    echo "  1. Check ST-Link connection"
    echo "  2. Verify Blue Pill is in programming mode"
    echo "  3. Try different USB cable/port"
    echo "  4. Check permissions: sudo usermod -a -G dialout \$USER"
    exit 1
fi
