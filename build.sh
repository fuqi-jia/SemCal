#!/bin/bash

# SemCal Build Script
# This script builds the SemCal project using CMake

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Default build type
BUILD_TYPE="Release"
BUILD_DIR="build"
CLEAN=false
JOBS=$(nproc 2>/dev/null || echo 4)
DOWNLOAD_SMTPARSER=true
SMTPARSER_URL="https://github.com/fuqi-jia/SMTParser.git"
SMTPARSER_DIR="external/SMTParser"

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -r|--release)
            BUILD_TYPE="Release"
            shift
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        --no-download)
            DOWNLOAD_SMTPARSER=false
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  -d, --debug      Build in Debug mode (default: Release)"
            echo "  -r, --release    Build in Release mode"
            echo "  -c, --clean      Clean build directory before building"
            echo "  -j, --jobs N     Number of parallel jobs (default: auto)"
            echo "  --no-download    Skip downloading SMTParser"
            echo "  -h, --help       Show this help message"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            echo "Use -h or --help for usage information"
            exit 1
            ;;
    esac
done

# Get the script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo -e "${GREEN}=== SemCal Build Script ===${NC}"
echo "Build type: $BUILD_TYPE"
echo "Build directory: $BUILD_DIR"
echo "Parallel jobs: $JOBS"
echo ""

# Clean if requested
if [ "$CLEAN" = true ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf "$BUILD_DIR"
fi

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Creating build directory...${NC}"
    mkdir -p "$BUILD_DIR"
fi

# Download SMTParser if needed
if [ "$DOWNLOAD_SMTPARSER" = true ]; then
    if [ ! -d "$SMTPARSER_DIR" ] || [ ! -d "$SMTPARSER_DIR/.git" ]; then
        echo -e "${YELLOW}Downloading SMTParser...${NC}"
        if [ -d "$SMTPARSER_DIR" ]; then
            # Remove incomplete directory
            rm -rf "$SMTPARSER_DIR"
        fi
        # Create external directory if it doesn't exist
        mkdir -p "$(dirname "$SMTPARSER_DIR")"
        # Clone SMTParser
        git clone "$SMTPARSER_URL" "$SMTPARSER_DIR" || {
            echo -e "${RED}Failed to download SMTParser${NC}"
            echo "You can skip this by using --no-download option"
            exit 1
        }
        echo -e "${GREEN}SMTParser downloaded successfully${NC}"
    else
        echo -e "${GREEN}SMTParser already exists${NC}"
    fi
fi

# Configure with CMake
echo -e "${YELLOW}Configuring with CMake...${NC}"
cd "$BUILD_DIR"
cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

# Build
echo -e "${YELLOW}Building...${NC}"
make -j"$JOBS"

echo ""
echo -e "${GREEN}=== Build Complete ===${NC}"
echo "Library: $BUILD_DIR/libsemcal.a"
echo "Examples:"
echo "  - $BUILD_DIR/examples/sat/sat_solver"
echo "  - $BUILD_DIR/examples/sat/smt_solver"
echo "  - $BUILD_DIR/examples/omt/omt_solver"
echo "  - $BUILD_DIR/examples/counting/counting_solver"
