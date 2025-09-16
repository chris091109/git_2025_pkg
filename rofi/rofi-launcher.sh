#!/bin/bash

# ~/.config/rofi/rofi-launcher.sh
# Simple search mode (default) - uses main config
rofi_simple() {
    rofi -show drun
}

# List mode with proper positioning and limited items
rofi_list() {
    rofi -show drun \
         -theme-str 'window { 
             location: south; 
             anchor: south;
             width: 500px; 
             height: 450px;
             y-offset: -50px;
         }' \
         -theme-str 'mainbox { 
             children: [ "inputbar", "listview" ]; 
             margin: 0px;
         }' \
         -theme-str 'inputbar {
             margin: 15px 15px 10px 15px;
         }' \
         -theme-str 'listview { 
             background-color: @background;
             margin: 0px 15px 15px 15px;
             lines: 10;
             scrollbar: true;
             scrollbar-width: 4px;
             fixed-height: true;
             columns: 1;
         }' \
         -theme-str 'element {
             padding: 8px;
             border-radius: 4px;
             background-color: transparent;
         }' \
         -theme-str 'element.alternate.normal {
             background-color: transparent;
         }' \
         -theme-str 'element selected {
             background-color: @selected;
             text-color: @foreground;
         }' \
         -theme-str 'element-text {
             text-color: @foreground;
         }' \
         -theme-str 'element-icon {
             size: 1.2em;
         }' \
         -theme-str 'scrollbar {
             background-color: #3c3836;
             handle-color: @background-alt;
             handle-width: 4px;
         }'
}
# Power menu
rofi_power() {
    # Create temporary power menu
    options="⏻ Power Off\n⭮ Restart\n⏾ Sleep"
    
    chosen=$(echo -e "$options" | rofi -dmenu -i -p "Power Menu" \
             -theme-str 'window { 
                 location: south; 
                 anchor: south;
                 width: 300px; 
                 height: 200px;
                 y-offset: -50px;
             }' \
             -theme-str 'mainbox {
                 children: [ "listview" ];
             }' \
             -theme-str 'listview { 
                 lines: 3;
                 fixed-height: true;
             }' \
             -theme-str 'element {
                 padding: 10px;
                 border-radius: 4px;
             }' \
             -theme-str 'element.alternate.normal {
                background-color: transparent;
             }' \
             -theme-str 'element selected {
                 background-color: @selected;
             }')
    
    case $chosen in
        "⏻ Power Off")
            systemctl poweroff
            ;;
        "⭮ Restart")
            systemctl reboot
            ;;
        "⏾ Sleep")
            systemctl suspend
            ;;
    esac
}

# Parse command line arguments
case "${1:-simple}" in
    simple|s)
        rofi_simple
        ;;
    list|l)
        rofi_list
        ;;
    power|p)
        rofi_power
        ;;
    *)
        echo "Usage: $0 [simple|list|power]"
        echo "  simple - Search only mode (default)"
        echo "  list   - List mode with sidebar"
        echo "  power  - Power management menu"
        exit 1
        ;;
esac
