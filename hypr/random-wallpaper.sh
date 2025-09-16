#!/bin/bash

WALLPAPERS_DIR="$HOME/Pictures/Wallpaper"

# Verify directory exists
if [ ! -d "$WALLPAPERS_DIR" ]; then
    notify-send "Wallpaper Error" "Directory not found: $WALLPAPERS_DIR" -u critical
    exit 1
fi

# Find all image files
WALLPAPERS=($(find "$WALLPAPERS_DIR" -type f \( -iname "*.jpg" -o -iname "*.png" -o -iname "*.jpeg" -o -iname "*.webp" \)))

# Check if any wallpapers were found
if [ ${#WALLPAPERS[@]} -eq 0 ]; then
    notify-send "Wallpaper Error" "No image files found in $WALLPAPERS_DIR" -u critical
    exit 1
fi

# Select random wallpaper
RANDOM_WALLPAPER=${WALLPAPERS[$RANDOM % ${#WALLPAPERS[@]}]}

# Debug output
echo "Trying to set wallpaper: $RANDOM_WALLPAPER" > /tmp/hyprpaper.log

# Preload wallpaper first
hyprctl hyprpaper preload "$RANDOM_WALLPAPER" >> /tmp/hyprpaper.log 2>&1

# Set the wallpaper with a small delay
sleep 0.1
if hyprctl hyprpaper wallpaper ",$RANDOM_WALLPAPER" >> /tmp/hyprpaper.log 2>&1; then
    notify-send "Wallpaper Changed" "$(basename "$RANDOM_WALLPAPER")" -i "$RANDOM_WALLPAPER"
else
    notify-send "Wallpaper Error" "Failed to set wallpaper. Check /tmp/hyprpaper.log" -u critical
fi
