#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

// ANSI color codes
const std::unordered_map<std::string, std::string> COLOR_MAP = {
    {"black", "\033[30m"},
    {"red", "\033[31m"},
    {"green", "\033[32m"},
    {"yellow", "\033[33m"},
    {"blue", "\033[34m"},
    {"magenta", "\033[35m"},
    {"cyan", "\033[36m"},
    {"white", "\033[37m"},
    {"bg_black", "\033[40m"},
    {"bg_red", "\033[41m"},
    {"bg_green", "\033[42m"},
    {"bg_yellow", "\033[43m"},
    {"bg_blue", "\033[44m"},
    {"bg_magenta", "\033[45m"},
    {"bg_cyan", "\033[46m"},
    {"bg_white", "\033[47m"},
    {"bold_black", "\033[1;30m"},
    {"bold_red", "\033[1;31m"},
    {"bold_green", "\033[1;32m"},
    {"bold_yellow", "\033[1;33m"},
    {"bold_blue", "\033[1;34m"},
    {"bold_magenta", "\033[1;35m"},
    {"bold_cyan", "\033[1;36m"},
    {"bold_white", "\033[1;37m"}
};

// Function to repeat a string `n` times
std::string repeat_string(const std::string& str, int n) {
    std::string result;
    for (int i = 0; i < n; ++i) {
        result += str;
    }
    return result;
}

// Function to draw a text box
void draw_box(const std::string& text, bool solid = false, const std::string& bg_color = "bg_blue", const std::string& text_color = "bold_white") {
    int text_length = text.length();
    int padding = 2; // Minimum space between text and box borders
    int box_width = text_length + 2 * padding; // Total box width

    // Calculate left and right spacing to center the text
    int total_spaces = box_width - text_length;
    int left_spaces = total_spaces / 2;
    int right_spaces = total_spaces - left_spaces;

    std::string horizontal_line = repeat_string("─", box_width);

    // Look up the ANSI color codes
    std::string bg_code = COLOR_MAP.count(bg_color) ? COLOR_MAP.at(bg_color) : COLOR_MAP.at("bg_blue");
    std::string text_code = COLOR_MAP.count(text_color) ? COLOR_MAP.at(text_color) : COLOR_MAP.at("bold_white");

    if (solid) {
        // Solid color box
        std::cout << bg_code; // Apply custom background color
        std::cout << text_code << "┌" << horizontal_line << "┐\n";
        std::cout << "│" << std::string(left_spaces, ' ') << text_code << text << bg_code << std::string(right_spaces, ' ') << "│\n";
        std::cout << "└" << horizontal_line << "┘\033[0m\n";
    } else {
        // Hollow box
        std::cout << text_code; // Apply custom text color
        std::cout << text_code << "┌" << horizontal_line << "┐\n";
        std::cout << "│" << std::string(left_spaces, ' ') << text << std::string(right_spaces, ' ') << "│\n";
        std::cout << "└" << horizontal_line << "┘\033[0m\n";
    }
}

// Function to validate color arguments
bool is_valid_color(const std::string& color, bool is_background = false) {
    std::string prefix = is_background ? "bg_" : "";
    return COLOR_MAP.count(prefix + color) > 0;
}

// Function to display help/usage instructions
void display_help() {
    draw_box("Help / Usage", false);
    std::cerr << "Usage: drawbox <text> [solid] [bg_color] [text_color]\n";
    std::cerr << "  <text>: The text to display in the box (required).\n";
    std::cerr << "  [solid]: Optional. Use 'solid' to create a solid box.\n";
    std::cerr << "  [bg_color]: Optional. Background color (e.g., 'bg_blue', 'bg_red').\n";
    std::cerr << "  [text_color]: Optional. Text color (e.g., 'white', 'green').\n";
    std::cerr << "  -l, --list: List all available colors.\n";
    std::cerr << "  --help: Display this help message.\n";
    std::cerr << "\nExample: drawbox \"Hello, World!\" solid bg_green white\n";
}

// Function to list available colors
void list_colors() {
    draw_box("Available Colors", false);
    // Extract keys from COLOR_MAP and sort them
    std::vector<std::string> colors;
    for (const auto& color : COLOR_MAP) {
        colors.push_back(color.first);
    }
    std::sort(colors.begin(), colors.end());

    // Print the sorted color names
    for (const auto& color : colors) {
        std::cout << "  " << color << "\n";
    }
    
}

int main(int argc, char* argv[]) {
    // Handle --help option
    if (argc == 2 && std::string(argv[1]) == "--help") {
        display_help();
        return 0;
    }

    // Handle --list or -l option
    if (argc == 2 && (std::string(argv[1]) == "--list" || std::string(argv[1]) == "-l")) {
        list_colors();
        return 0;
    }

    // Validate the number of arguments
    if (argc < 2) {
        std::cerr << "Error: Missing required argument <text>.\n";
        display_help();
        return 1;
    }

    std::string text = argv[1];
    bool solid = false;
    std::string bg_color = "bg_blue";
    std::string text_color = "bold_white";

    // Parse optional arguments
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "solid") {
            solid = true;
        } else if (arg.substr(0, 3) == "bg_") {
            // Background color
            if (!is_valid_color(arg.substr(3), true)) {
                std::cerr << "Error: Invalid background color '" << arg << "'.\n";
                display_help();
                return 1;
            }
            bg_color = arg;
        } else {
            // Text color
            if (!is_valid_color(arg)) {
                std::cerr << "Error: Invalid text color '" << arg << "'.\n";
                display_help();
                return 1;
            }
            text_color = arg;
        }
    }

    // Draw the box
    try {
        draw_box(text, solid, bg_color, text_color);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
