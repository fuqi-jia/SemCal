#!/bin/bash

# SemCal Auto Commit Script
# Edit the COMMIT_MESSAGE variable below to change the commit message

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# ============================================
# EDIT THIS MESSAGE TO CHANGE COMMIT MESSAGE
# ============================================
COMMIT_MESSAGE="Update SemCal: fix GitHub markdown compatibility and add SMTParser auto-download"

# Optional: Set to true to automatically push after commit
AUTO_PUSH=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -m|--message)
            COMMIT_MESSAGE="$2"
            shift 2
            ;;
        -p|--push)
            AUTO_PUSH=true
            shift
            ;;
        --no-push)
            AUTO_PUSH=false
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  -m, --message MSG  Override commit message"
            echo "  -p, --push         Automatically push after commit"
            echo "  --no-push          Don't push (default)"
            echo "  -h, --help         Show this help message"
            echo ""
            echo "To change the default commit message, edit COMMIT_MESSAGE in this script."
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

echo -e "${GREEN}=== SemCal Auto Commit ===${NC}"
echo ""

# Check if we're in a git repository
if ! git rev-parse --git-dir > /dev/null 2>&1; then
    echo -e "${RED}Error: Not in a git repository${NC}"
    exit 1
fi

# Check if there are changes to commit
if git diff --quiet && git diff --cached --quiet; then
    echo -e "${YELLOW}No changes to commit${NC}"
    exit 0
fi

# Show status
echo -e "${YELLOW}Current status:${NC}"
git status --short
echo ""

# Add all changes
echo -e "${YELLOW}Staging all changes...${NC}"
git add .

# Commit
echo -e "${YELLOW}Committing with message:${NC}"
echo -e "${GREEN}$COMMIT_MESSAGE${NC}"
echo ""
git commit -m "$COMMIT_MESSAGE"

# Push if requested
if [ "$AUTO_PUSH" = true ]; then
    echo ""
    echo -e "${YELLOW}Pushing to remote...${NC}"
    git push
    echo -e "${GREEN}Pushed successfully${NC}"
else
    echo ""
    echo -e "${YELLOW}Commit completed. Use 'git push' to push to remote.${NC}"
    echo "Or run with -p/--push flag to auto-push: $0 -p"
fi

echo ""
echo -e "${GREEN}=== Commit Complete ===${NC}"
