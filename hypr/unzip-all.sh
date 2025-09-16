#!/bin/bash

# Script: unzip-all
# Description: Handles multiple archive formats commonly found on Linux

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to display usage
usage() {
    echo -e "${GREEN}Usage: $0 [archive_file] [options]${NC}"
    echo "Options:"
    echo "  -d, --destination DIR    Extract to specific directory"
    echo "  -l, --list               List contents without extracting"
    echo "  -v, --verbose            Show verbose output"
    echo "  -h, --help               Show this help message"
    echo ""
    echo "Supported formats: zip, tar, tar.gz, tar.bz2, tar.xz, rar, 7z, deb, cpio"
}

# Function to check if command exists
check_command() {
    if ! command -v "$1" &> /dev/null; then
        echo -e "${RED}Error: $1 is not installed. Please install it to handle $2 files.${NC}"
        return 1
    fi
    return 0
}

# Function to extract archive
extract_archive() {
    local file="$1"
    local dest="$2"
    local list_only="$3"
    local verbose="$4"
    
    local v_flag=""
    if [ "$verbose" = "true" ]; then
        v_flag="v"
    fi
    
    case "${file##*.}" in
        zip)
            check_command unzip "ZIP" || return 1
            if [ "$list_only" = "true" ]; then
                unzip -l "$file"
            else
                unzip $([ "$verbose" = "true" ] && echo "-v") "$file" -d "${dest:-.}"
            fi
            ;;
        tar)
            if [ "$list_only" = "true" ]; then
                tar -tf "$file"
            else
                tar -x${v_flag}f "$file" -C "${dest:-.}"
            fi
            ;;
        gz|tgz)
            if [ "$list_only" = "true" ]; then
                tar -tzf "$file"
            else
                tar -x${v_flag}zf "$file" -C "${dest:-.}"
            fi
            ;;
        bz2|tbz2)
            if [ "$list_only" = "true" ]; then
                tar -tjf "$file"
            else
                tar -x${v_flag}jf "$file" -C "${dest:-.}"
            fi
            ;;
        xz|txz)
            check_command xz "XZ" || return 1
            if [ "$list_only" = "true" ]; then
                tar -tJf "$file"
            else
                tar -x${v_flag}Jf "$file" -C "${dest:-.}"
            fi
            ;;
        rar)
            check_command unrar "RAR" || check_command rar "RAR" || return 1
            if command -v unrar &> /dev/null; then
                if [ "$list_only" = "true" ]; then
                    unrar l "$file"
                else
                    unrar x $([ "$verbose" = "true" ] && echo "-v") "$file" "${dest:-.}"
                fi
            else
                if [ "$list_only" = "true" ]; then
                    rar l "$file"
                else
                    rar x $([ "$verbose" = "true" ] && echo "-v") "$file" "${dest:-.}"
                fi
            fi
            ;;
        7z)
            check_command 7z "7-Zip" || return 1
            if [ "$list_only" = "true" ]; then
                7z l "$file"
            else
                7z x $([ "$verbose" = "true" ] && echo "-r") "$file" -o"${dest:-.}"
            fi
            ;;
        deb)
            if [ "$list_only" = "true" ]; then
                dpkg-deb -c "$file"
            else
                dpkg-deb -x "$file" "${dest:-.}"
            fi
            ;;
        cpio)
            if [ "$list_only" = "true" ]; then
                cpio -it < "$file"
            else
                cpio -id${v_flag} < "$file" -D "${dest:-.}"
            fi
            ;;
        *)
            # Try to detect format by file command
            local file_type=$(file -b "$file")
            case "$file_type" in
                *"Zip archive"*)
                    if [ "$list_only" = "true" ]; then
                        unzip -l "$file"
                    else
                        unzip $([ "$verbose" = "true" ] && echo "-v") "$file" -d "${dest:-.}"
                    fi
                    ;;
                *"tar archive"*)
                    if [ "$list_only" = "true" ]; then
                        tar -tf "$file"
                    else
                        tar -x${v_flag}f "$file" -C "${dest:-.}"
                    fi
                    ;;
                *"gzip compressed"*)
                    if [ "$list_only" = "true" ]; then
                        tar -tzf "$file"
                    else
                        tar -x${v_flag}zf "$file" -C "${dest:-.}"
                    fi
                    ;;
                *"bzip2 compressed"*)
                    if [ "$list_only" = "true" ]; then
                        tar -tjf "$file"
                    else
                        tar -x${v_flag}jf "$file" -C "${dest:-.}"
                    fi
                    ;;
                *"XZ compressed"*)
                    if [ "$list_only" = "true" ]; then
                        tar -tJf "$file"
                    else
                        tar -x${v_flag}Jf "$file" -C "${dest:-.}"
                    fi
                    ;;
                *)
                    echo -e "${RED}Error: Unsupported file format or unknown archive type: $file${NC}"
                    echo -e "${YELLOW}File type detected: $file_type${NC}"
                    return 1
                    ;;
            esac
            ;;
    esac
}

# Main script
main() {
    local file=""
    local dest=""
    local list_only="false"
    local verbose="false"
    
    # Parse arguments
    while [[ $# -gt 0 ]]; do
        case "$1" in
            -d|--destination)
                dest="$2"
                shift 2
                ;;
            -l|--list)
                list_only="true"
                shift
                ;;
            -v|--verbose)
                verbose="true"
                shift
                ;;
            -h|--help)
                usage
                exit 0
                ;;
            -*)
                echo -e "${RED}Error: Unknown option $1${NC}"
                usage
                exit 1
                ;;
            *)
                file="$1"
                shift
                ;;
        esac
    done
    
    # Check if file was provided
    if [ -z "$file" ]; then
        echo -e "${RED}Error: No archive file specified${NC}"
        usage
        exit 1
    fi
    
    # Check if file exists
    if [ ! -f "$file" ]; then
        echo -e "${RED}Error: File '$file' does not exist${NC}"
        exit 1
    fi
    
    # Create destination directory if it doesn't exist
    if [ -n "$dest" ] && [ ! -d "$dest" ]; then
        mkdir -p "$dest"
    fi
    
    echo -e "${BLUE}Processing: $file${NC}"
    if [ -n "$dest" ]; then
        echo -e "${BLUE}Destination: $dest${NC}"
    fi
    
    # Extract the archive
    if extract_archive "$file" "$dest" "$list_only" "$verbose"; then
        echo -e "${GREEN}Successfully processed: $file${NC}"
    else
        echo -e "${RED}Failed to process: $file${NC}"
        exit 1
    fi
}

# Run main function with all arguments
main "$@"
