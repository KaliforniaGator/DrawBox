#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <wchar.h>
#include <locale.h>
#include <fstream>

// ===== Unicode Character Sets =====
// Create a namespace for your unicode libraries
namespace unicode {
    // Box drawing characters - basic set
    const std::unordered_map<std::string, std::string> BOX_CHARS = {
        // Simple box drawing
        {"h_line", "─"}, {"v_line", "│"}, 
        {"tl_corner", "┌"}, {"tr_corner", "┐"}, {"bl_corner", "└"}, {"br_corner", "┘"},
        {"t_down", "┬"}, {"t_up", "┴"}, {"t_right", "├"}, {"t_left", "┤"}, {"cross", "┼"},
        
        // Double line box drawing
        {"d_h_line", "═"}, {"d_v_line", "║"}, 
        {"d_tl_corner", "╔"}, {"d_tr_corner", "╗"}, {"d_bl_corner", "╚"}, {"d_br_corner", "╝"},
        {"d_t_down", "╦"}, {"d_t_up", "╩"}, {"d_t_right", "╠"}, {"d_t_left", "╣"}, {"d_cross", "╬"},
        
        // Rounded corners
        {"r_tl_corner", "╭"}, {"r_tr_corner", "╮"}, {"r_bl_corner", "╰"}, {"r_br_corner", "╯"},
        
        // Heavy lines
        {"heavy_h", "━"}, {"heavy_v", "┃"},
        {"heavy_tl", "┏"}, {"heavy_tr", "┓"}, {"heavy_bl", "┗"}, {"heavy_br", "┛"},
        
        // Block elements
        {"block_full", "█"}, {"block_half", "▄"}, {"block_light", "░"}, {"block_medium", "▒"}, {"block_dark", "▓"},
        
        // Arrows
        {"arrow_up", "↑"}, {"arrow_down", "↓"}, {"arrow_left", "←"}, {"arrow_right", "→"},
        {"arrow_up_down", "↕"}, {"arrow_left_right", "↔"},
        
        // Geometric shapes
        {"circle", "●"}, {"triangle", "▲"}, {"square", "■"}, {"diamond", "◆"},
        
        // Other useful symbols
        {"check", "✓"}, {"cross_mark", "✗"}, {"bullet", "•"}, {"star", "★"}
    };
    
    // Emoji characters
    const std::unordered_map<std::string, std::string> EMOJI = {
        // Faces
        {"smile", "😊"}, {"laugh", "😄"}, {"sad", "😢"}, {"angry", "😠"}, {"think", "🤔"},
        
        // Common objects
        {"heart", "❤️"}, {"fire", "🔥"}, {"thumbs_up", "👍"}, {"thumbs_down", "👎"},
        {"clap", "👏"}, {"gift", "🎁"}, {"bulb", "💡"}, {"book", "📚"},
        
        // Weather
        {"sun", "☀️"}, {"cloud", "☁️"}, {"rain", "🌧️"}, {"snow", "❄️"}, {"storm", "⚡"},
        
        // Tech
        {"computer", "💻"}, {"phone", "📱"}, {"email", "📧"}, {"camera", "📷"},
        
        // Nature
        {"tree", "🌳"}, {"flower", "🌸"}, {"mountain", "⛰️"}, {"ocean", "🌊"}
    };
    
    // Icons for UI elements
    const std::unordered_map<std::string, std::string> UI_ICONS = {
        {"settings", "⚙️"}, {"search", "🔍"}, {"user", "👤"}, {"home", "🏠"},
        {"menu", "☰"}, {"close", "✕"}, {"add", "➕"}, {"remove", "➖"},
        {"info", "ℹ️"}, {"warning", "⚠️"}, {"error", "⛔"}, {"success", "✅"},
        {"calendar", "📅"}, {"clock", "🕒"}, {"lock", "🔒"}, {"unlock", "🔓"},
        {"play", "▶️"}, {"pause", "⏸️"}, {"stop", "⏹️"}, {"next", "⏭️"}, {"prev", "⏮️"}
    };
}

// ANSI color codes
const std::unordered_map<std::string, std::string> COLOR_MAP = {
    {"black", "\033[30m"},
    {"red", "\033[31m"},
    {"green", "\033[32m"},
    {"yellow", "\033[33m"},
    {"blue", "\033[34m"}, // Foreground blue
    {"magenta", "\033[35m"},
    {"cyan", "\033[36m"},
    {"white", "\033[37m"},
    {"bg_black", "\033[40m"},
    {"bg_red", "\033[41m"},
    {"bg_green", "\033[42m"},
    {"bg_yellow", "\033[43m"},
    {"bg_blue", "\033[44m"}, // Background blue
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
    {"bold_white", "\033[1;37m"}};

// Function to repeat a string `n` times
std::string repeat_string(const std::string &str, int n)
{
    std::string result;
    for (int i = 0; i < n; ++i)
    {
        result += str;
    }
    return result;
}

// Function to calculate the visible length of a string, ignoring ANSI escape sequences
size_t visible_length(const std::string &str) {
    size_t visible_len = 0;
    bool in_escape = false;
    
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '\033') {
            in_escape = true;
            continue;
        }
        
        if (in_escape) {
            if (str[i] == 'm') {
                in_escape = false;
            }
            continue;
        }
        
        // Only count visible characters
        visible_len++;
    }
    
    return visible_len;
}


// Function to draw a text box
void draw_box(const std::string &text, bool solid = false, const std::string &bg_color = "bg_blue", const std::string &text_color = "bold_white")
{
    int text_length = text.length();
    int padding = 2;
    int box_width = text_length + 2 * padding;

    int total_spaces = box_width - text_length;
    int left_spaces = total_spaces / 2;
    int right_spaces = total_spaces - left_spaces;

    std::string horizontal_line = repeat_string("─", box_width);

    std::string bg_code = COLOR_MAP.count(bg_color) ? COLOR_MAP.at(bg_color) : COLOR_MAP.at("bg_blue");
    std::string text_code = COLOR_MAP.count(text_color) ? COLOR_MAP.at(text_color) : COLOR_MAP.at("bold_white");
    std::string reset_code = "\033[0m";

    if (solid)
    {
        std::cout << bg_code << text_code << "┌" << horizontal_line << "┐" << reset_code << "\n";
        std::cout << bg_code << text_code << "│" << std::string(left_spaces, ' ') << text_code << text << bg_code << text_code << std::string(right_spaces, ' ') << "│" << reset_code << "\n";
        std::cout << bg_code << text_code << "└" << horizontal_line << "┘" << reset_code << "\n";
    }
    else
    {
        std::cout << text_code << "┌" << horizontal_line << "┐" << reset_code << "\n";
        std::cout << text_code << "│" << std::string(left_spaces, ' ') << text << std::string(right_spaces, ' ') << "│" << reset_code << "\n";
        std::cout << text_code << "└" << horizontal_line << "┘" << reset_code << "\n";
    }
}

// Function to draw a table with properly adjusted widths for ANSI escape codes
void draw_table(const std::vector<std::vector<std::string>> &rows, bool solid_mode = false,
    bool hollow_mode = false, const std::string &bg_color = "bg_blue",
    const std::string &text_color = "bold_white")
{
    // Unicode box drawing characters
    const std::string border_chars[] = {
        "┌", "┐", "└", "┘", "─", "│", "┬", "┴", "├", "┤", "┼" 
    };
    enum BorderIndex { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, 
              HORIZONTAL, VERTICAL, TOP_T, BOTTOM_T, LEFT_T, RIGHT_T, CROSS };

    // Color setup
    std::string bg_code = solid_mode && COLOR_MAP.count(bg_color) ? COLOR_MAP.at(bg_color) : "";
    std::string text_code = solid_mode && COLOR_MAP.count(text_color) ? COLOR_MAP.at(text_color) : "";
    std::string border_color_code = hollow_mode && COLOR_MAP.count(text_color) ? COLOR_MAP.at(text_color) : "";
    const std::string reset_code = "\033[0m";

    // Improved function to calculate visible length without ANSI escape codes
    auto visible_length = [](const std::string &str) -> size_t {
        size_t visible_len = 0;
        bool in_escape = false;
        
        for (size_t i = 0; i < str.length(); i++) {
            if (str[i] == '\033') {
                in_escape = true;
                continue;
            }
            
            if (in_escape) {
                if (str[i] == 'm') {
                    in_escape = false;
                }
                continue;
            }
            
            // Only count visible characters
            visible_len++;
        }
        
        return visible_len;
    };

    // Helper function to repeat a string
    auto repeat_string = [](const std::string &str, size_t n) -> std::string {
        std::string result;
        for (size_t i = 0; i < n; i++) {
            result += str;
        }
        return result;
    };

    // Calculate max visible column widths, accounting for ANSI escape sequences
    std::vector<size_t> column_widths;
    
    // First find the maximum number of columns in any row
    size_t max_columns = 0;
    for (const auto &row : rows) {
        max_columns = std::max(max_columns, row.size());
    }
    
    // Initialize column_widths with zeros
    column_widths.resize(max_columns, 0);
    
    // Calculate the maximum width for each column
    for (const auto &row : rows) {
        for (size_t i = 0; i < row.size(); ++i) {
            // Calculate visible length (excluding ANSI escape codes)
            size_t text_length = visible_length(row[i]);
            column_widths[i] = std::max(column_widths[i], text_length);
        }
    }

    // Helper function to draw horizontal borders
    auto draw_horizontal_border = [&](BorderIndex left, BorderIndex middle, BorderIndex right) {
        std::string border_start = (solid_mode ? bg_code + text_code : 
                               (hollow_mode ? border_color_code : ""));

        std::cout << border_start << border_chars[left];

        for (size_t i = 0; i < column_widths.size(); ++i) {
            std::cout << repeat_string(border_chars[HORIZONTAL], column_widths[i] + 2);
            if (i < column_widths.size() - 1) {
                std::cout << border_chars[middle];
            }
        }

        std::cout << border_chars[right];
        if (!border_start.empty()) std::cout << reset_code;
        std::cout << "\n";
    };

    // Draw top border
    draw_horizontal_border(TOP_LEFT, TOP_T, TOP_RIGHT);

    // Draw rows
    for (size_t row_idx = 0; row_idx < rows.size(); ++row_idx) {
        const auto &row = rows[row_idx];

        // Output row content
        std::string border_start = (solid_mode ? bg_code + text_code : 
                               (hollow_mode ? border_color_code : ""));

        std::cout << border_start << border_chars[VERTICAL];
        if (!border_start.empty() && !solid_mode) std::cout << reset_code;

        // Ensure we handle all columns, even if this row has fewer
        for (size_t i = 0; i < column_widths.size(); ++i) {
            std::string cell_content = (i < row.size()) ? row[i] : "";
            size_t visible_length_value = visible_length(cell_content);
            
            // Calculate needed padding
            int padding_needed = static_cast<int>(column_widths[i]) - static_cast<int>(visible_length_value);
            
            if (solid_mode) {
                // In solid mode, maintain the background color for cell values
                std::cout << " " << cell_content;
                
                // Apply padding after the cell content, accounting for ANSI codes
                if (padding_needed > 0) {
                    std::cout << repeat_string(" ", padding_needed);
                }
                
                std::cout << " " << border_chars[VERTICAL];

                if (i >= row.size() - 1) {
                    std::cout << reset_code;
                }
            } else {
                // Normal mode or hollow mode
                std::cout << " " << cell_content;
                
                // Apply padding after the cell content, accounting for ANSI codes
                if (padding_needed > 0) {
                    std::cout << repeat_string(" ", padding_needed);
                }
                
                std::cout << " ";
                
                // Add vertical border with appropriate color
                if (hollow_mode) {
                    std::cout << border_color_code << border_chars[VERTICAL] << reset_code;
                } else {
                    std::cout << border_chars[VERTICAL];
                }
            }
        }

        std::cout << "\n";

        // Draw horizontal separator after each row except the last
        if (row_idx < rows.size() - 1) {
            draw_horizontal_border(LEFT_T, CROSS, RIGHT_T);
        }
    }

    // Draw bottom border
    draw_horizontal_border(BOTTOM_LEFT, BOTTOM_T, BOTTOM_RIGHT);
}

// Function to draw a banner
void draw_banner(const std::string &text, const std::string &bg_color = "bg_blue", const std::string &text_color = "bold_white")
{
    int text_length = text.length();
    int padding = 4;                              // Padding around the text
    int banner_width = text_length + 2 * padding; // Total width of the banner

    // Calculate left and right padding for center alignment of text
    int total_spaces = banner_width - text_length;
    int left_spaces = total_spaces / 2;
    int right_spaces = total_spaces - left_spaces;

    // Horizontal line decoration
    std::string horizontal_line = repeat_string("━", banner_width);

    // Color codes
    std::string bg_code = COLOR_MAP.count(bg_color) ? COLOR_MAP.at(bg_color) : COLOR_MAP.at("bg_blue");
    std::string text_code = COLOR_MAP.count(text_color) ? COLOR_MAP.at(text_color) : COLOR_MAP.at("bold_white");
    std::string reset_code = "\033[0m";

    // Print the top part of the banner
    std::cout << bg_code << text_code << "╭" << horizontal_line << "╮" << reset_code << "\n"; // Top corners and horizontal line

    // Print the text with padding
    std::cout << bg_code << text_code << "│" << std::string(left_spaces, ' ') << text << std::string(right_spaces, ' ') << "│" << reset_code << "\n"; // Side borders

    // Print the bottom part of the banner
    std::cout << bg_code << text_code << "╰" << horizontal_line << "╯" << reset_code << "\n"; // Bottom corners and horizontal line
}

// Function to draw a textbox and capture user input
std::string get_input_for_textbox(const std::string &title)
{
    std::string content;
    std::cout << title << " (enter content): ";
    std::getline(std::cin, content); // Capture user input
    return content;
}

void draw_textbox(const std::string &label, const std::string &bg_color = "bg_blue", const std::string &text_color = "bold_white")
{
    // Retrieve the color codes
    std::string bg_code = COLOR_MAP.count(bg_color) ? COLOR_MAP.at(bg_color) : COLOR_MAP.at("bg_blue");
    std::string text_code = COLOR_MAP.count(text_color) ? COLOR_MAP.at(text_color) : COLOR_MAP.at("bold_white");

    // Derive the arrow color from the background color by removing the "bg_" prefix
    std::string arrow_color = bg_color.substr(3); // Remove "bg_" to get the arrow color

    std::string arrow_code = COLOR_MAP.count(arrow_color) ? COLOR_MAP.at(arrow_color) : COLOR_MAP.at("blue");
    std::string resetColor = "\033[0m";

    // Print the label with the background color to stderr
    std::cerr << bg_code << text_code << " " << label << " ";
    std::cerr << resetColor; // Reset after label

    // Print the title end cap with the specified arrow color to stderr
    std::cerr << arrow_code << " " << text_code; // Ensure this is a valid Unicode character

    // Allow user input
    std::string input;
    std::getline(std::cin, input); // Capture user input

    // Print the user input to stdout (this will be redirected to the file)
    std::cout << input << std::endl;
}

// Function to validate color arguments
bool is_valid_color(const std::string &color, bool is_background = false)
{
    std::string prefix = is_background ? "bg_" : "";
    return COLOR_MAP.count(prefix + color) > 0;
}

// Function to display help/usage instructions
void display_help()
{
    std::cerr << "Usage: drawbox [mode] <arguments>...\n";
    std::cerr << "\nModes (optional, default is 'box'):\n";
    std::cerr << "  box <text> [solid] [bg_color] [text_color]  - Draw a box around the text.\n";
    std::cerr << "      <text>: Text to display inside the box.\n";
    std::cerr << "      [solid]: Optional. Use 'solid' for a solid box (default is hollow).\n";
    std::cerr << "      [bg_color]: Optional. Background color (e.g., 'bg_blue', 'bg_red').\n";
    std::cerr << "      [text_color]: Optional. Text color (e.g., 'white', 'green').\n";
    std::cerr << "\n";
    std::cerr << "  table <row1> <row2> ... [solid|hollow] [bg_color] [text_color]  - Draw a table with comma-separated rows.\n";
    std::cerr << "      <row>: A row of comma-separated values (e.g., 'Name,Age').\n";
    std::cerr << "      [solid]: Optional. Use 'solid' for a solid colored table (with background color).\n";
    std::cerr << "      [hollow]: Optional. Use 'hollow' for a table with colored borders (no background color).\n";
    std::cerr << "      [bg_color]: Optional. Background color for solid mode (e.g., 'bg_blue', 'bg_red').\n";
    std::cerr << "      [text_color]: Optional. Text color for solid mode or border color for hollow mode (e.g., 'white', 'green').\n";
    std::cerr << "\n";
    std::cerr << "  banner <text> [bg_color] [text_color]  - Draw a banner with the given text.\n";
    std::cerr << "      <text>: Text to display in the banner.\n";
    std::cerr << "      [bg_color]: Optional. Background color (e.g., 'bg_blue', 'bg_red').\n";
    std::cerr << "      [text_color]: Optional. Text color (e.g., 'white', 'green').\n";
    std::cerr << "\n";
    std::cerr << "  textbox <title> [bg_color] [text_color]  - Prompt with title and capture user input.\n";
    std::cerr << "      <title>: The prompt text to display.\n";
    std::cerr << "      [bg_color]: Optional. Background color (e.g., 'bg_blue', 'bg_red').\n";
    std::cerr << "      [text_color]: Optional. Text color (e.g., 'white', 'green').\n";
    std::cerr << "      Note: The textbox mode output can be redirected (e.g., `drawbox textbox \"Title:\" > output.txt`).\n";
    std::cerr << "\n";
    std::cerr << "  showcase  - Display examples of all drawbox features.\n";

    std::cerr << "  progress <value> <max> [width] [fill_char] [empty_char] [text_color]  - Draw a progress bar.\n";
    std::cerr << "      <value>: Current value.\n";
    std::cerr << "      <max>: Maximum value.\n";
    std::cerr << "      [width]: Optional. Width of the progress bar (default 20).\n";
    std::cerr << "      [fill_char]: Optional. Character name for filled portion (e.g., 'block_full').\n";
    std::cerr << "      [empty_char]: Optional. Character name for empty portion (e.g., 'block_light').\n";
    std::cerr << "      [text_color]: Optional. Text color (e.g., 'white', 'green').\n";
    
    std::cerr << "  art <file>  - Draw Unicode art from a file.\n";
    std::cerr << "      <file>: Path to the art file with {name} placeholders for Unicode characters.\n";
    
    std::cerr << "  tooltip <text> [icon] [double] [text_color]  - Draw a tooltip.\n";
    std::cerr << "      <text>: Text to display in the tooltip.\n";
    std::cerr << "      [icon]: Optional. Icon name (e.g., 'info', 'warning').\n";
    std::cerr << "      [double]: Optional. Use 'double' for double-line style.\n";
    std::cerr << "      [text_color]: Optional. Text color (e.g., 'white', 'green').\n";
    
    std::cerr << "  calendar <month> <year> [text_color]  - Draw a calendar.\n";
    std::cerr << "      <month>: Month (1-12).\n";
    std::cerr << "      <year>: Year.\n";
    std::cerr << "      [text_color]: Optional. Text color (e.g., 'white', 'green').\n";
    
    std::cerr << "  unicode <name>  - Print a Unicode character by name.\n";
    std::cerr << "      <name>: Name of the Unicode character (e.g., 'check', 'smile').\n";
    
    std::cerr << "  list_unicode [category]  - List available Unicode characters.\n";
    std::cerr << "      [category]: Optional. Category to list ('box', 'emoji', 'ui').\n";

    std::cerr << "\nExamples:\n";
    std::cerr << "  drawbox \"Hello, World!\" solid bg_green white\n";
    std::cerr << "  drawbox table \"Name,Age\" \"John,25\" \"Alice,30\"\n";
    std::cerr << "  drawbox table \"Name,Age\" \"John,25\" \"Alice,30\" color bg_cyan white\n";
    std::cerr << "  drawbox banner \"Welcome to My Program!\" bg_magenta white\n";
    std::cerr << "  drawbox textbox \"Enter your name:\" bg_cyan white\n";
    std::cerr << "  drawbox textbox \"Title:\" > hello.txt\n";
    std::cerr << "  drawbox showcase\n";
    std::cerr << "  drawbox progress 50 100 30 block_full block_light white\n";
    std::cerr << "  drawbox art art.txt\n";
    std::cerr << "  drawbox tooltip \"This is a tooltip\" info double white\n";
    std::cerr << "  drawbox calendar 12 2021 white\n";
    std::cerr << "  drawbox unicode smile\n";
    std::cerr << "  drawbox list_unicode emoji\n";

}

// ===== New Drawing Functions =====

// Function to get a Unicode character by name
std::string get_unicode(const std::string &name) {
    // Check in all character sets
    if (unicode::BOX_CHARS.count(name) > 0) {
        return unicode::BOX_CHARS.at(name);
    } else if (unicode::EMOJI.count(name) > 0) {
        return unicode::EMOJI.at(name);
    } else if (unicode::UI_ICONS.count(name) > 0) {
        return unicode::UI_ICONS.at(name);
    }
    // Return the name itself if not found (fallback)
    return name;
}

// Draw a progress bar with customizable characters
void draw_progress_bar(int value, int max, int width = 20, 
                      const std::string &filled_char = "block_full", 
                      const std::string &empty_char = "block_light",
                      const std::string &text_color = "bold_white") {
    // Calculate percentage and bar fill
    int percentage = (max > 0) ? (value * 100) / max : 0;
    int filled_width = (width * value) / max;
    
    // Get color codes
    std::string text_code = COLOR_MAP.count(text_color) ? COLOR_MAP.at(text_color) : COLOR_MAP.at("bold_white");
    std::string reset_code = "\033[0m";
    
    // Get Unicode characters
    std::string filled = get_unicode(filled_char);
    std::string empty = get_unicode(empty_char);
    
    // Draw the progress bar
    std::cout << text_code << "[";
    for (int i = 0; i < width; i++) {
        if (i < filled_width) {
            std::cout << filled;
        } else {
            std::cout << empty;
        }
    }
    std::cout << "] " << percentage << "%" << reset_code << std::endl;
}

// Function to draw a simple Unicode art
void draw_unicode_art(const std::vector<std::string> &art_rows, 
                     const std::string &text_color = "bold_white") {
    // Get color code
    std::string text_code = COLOR_MAP.count(text_color) ? COLOR_MAP.at(text_color) : COLOR_MAP.at("bold_white");
    std::string reset_code = "\033[0m";
    
    // Replace unicode names with actual characters and print
    for (const auto &row : art_rows) {
        std::string processed_row = row;
        
        // Look for patterns like {name} and replace with Unicode
        size_t start_pos = 0;
        while ((start_pos = processed_row.find("{", start_pos)) != std::string::npos) {
            size_t end_pos = processed_row.find("}", start_pos);
            if (end_pos == std::string::npos) break;
            
            std::string key = processed_row.substr(start_pos + 1, end_pos - start_pos - 1);
            std::string replacement = get_unicode(key);
            
            processed_row.replace(start_pos, end_pos - start_pos + 1, replacement);
            start_pos += replacement.length();
        }
        
        std::cout << text_code << processed_row << reset_code << std::endl;
    }
}

// Function to draw a tooltip or notification bubble
void draw_tooltip(const std::string &text, const std::string &icon = "info",
                 bool use_double_line = false, const std::string &text_color = "bold_white") {
    std::string icon_char = get_unicode(icon);
    int text_length = visible_length(text);
    int padding = 2;
    int box_width = text_length + 2 * padding + 2; // +2 for the icon
    
    // Get color codes
    std::string text_code = COLOR_MAP.count(text_color) ? COLOR_MAP.at(text_color) : COLOR_MAP.at("bold_white");
    std::string reset_code = "\033[0m";
    
    // Choose box characters based on style
    std::string h_line = use_double_line ? get_unicode("d_h_line") : get_unicode("h_line");
    std::string v_line = use_double_line ? get_unicode("d_v_line") : get_unicode("v_line");
    std::string tl_corner = use_double_line ? get_unicode("d_tl_corner") : get_unicode("tl_corner");
    std::string tr_corner = use_double_line ? get_unicode("d_tr_corner") : get_unicode("tr_corner");
    std::string bl_corner = use_double_line ? get_unicode("d_bl_corner") : get_unicode("bl_corner");
    std::string br_corner = use_double_line ? get_unicode("d_br_corner") : get_unicode("br_corner");
    
    // Generate the horizontal line
    std::string horizontal_line = repeat_string(h_line, box_width);
    
    // Draw the tooltip
    std::cout << text_code << tl_corner << horizontal_line << tr_corner << reset_code << std::endl;
    std::cout << text_code << v_line << " " << icon_char << " " << text << "  " << v_line << reset_code << std::endl;
    std::cout << text_code << bl_corner << horizontal_line << br_corner << reset_code << std::endl;
}

// Draw a calendar for the given month and year
void draw_calendar(int month, int year, const std::string &text_color = "bold_white") {
    // Function to determine the day of week for a date (Zeller's congruence)
    auto day_of_week = [](int d, int m, int y) -> int {
        if (m < 3) {
            m += 12;
            y--;
        }
        int k = y % 100;
        int j = y / 100;
        return (d + 13 * (m + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    };
    
    // Function to get days in a month
    auto days_in_month = [](int m, int y) -> int {
        if (m == 2) {
            bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
            return leap ? 29 : 28;
        } else if (m == 4 || m == 6 || m == 9 || m == 11) {
            return 30;
        } else {
            return 31;
        }
    };
    
    // Month names
    std::vector<std::string> month_names = {
        "January", "February", "March", "April", "May", "June", 
        "July", "August", "September", "October", "November", "December"
    };
    
    // Get color code
    std::string text_code = COLOR_MAP.count(text_color) ? COLOR_MAP.at(text_color) : COLOR_MAP.at("bold_white");
    std::string reset_code = "\033[0m";
    
    // Create title
    std::string title = month_names[month - 1] + " " + std::to_string(year);
    
    // Get first day of the month
    int first_day = day_of_week(1, month, year);
    // Sunday is 0 in our formula but we want it to be 6 (last day of week)
    first_day = (first_day + 6) % 7;
    
    // Get number of days in the month
    int days = days_in_month(month, year);

    // Choose box characters based on style
    std::string h_line =  get_unicode("h_line");
    std::string v_line =  get_unicode("v_line");
    std::string tl_corner =  get_unicode("tl_corner");
    std::string tr_corner =  get_unicode("tr_corner");
    std::string bl_corner =  get_unicode("bl_corner");
    std::string br_corner =  get_unicode("br_corner");
    std::string t_right =  get_unicode("t_right");
    std::string t_left =  get_unicode("t_left");
    
    // Draw calendar
    std::cout << text_code << title << reset_code << std::endl;
    std::cout << text_code << tl_corner << h_line << repeat_string(h_line, 20) << tr_corner << reset_code << std::endl;
    std::cout << text_code << v_line << " Mo Tu We Th Fr Sa Su" << v_line << reset_code << std::endl;
    std::cout << text_code << t_right << h_line << repeat_string(h_line, 20) << t_left <<reset_code << std::endl;
    
    // Print days
    int day_counter = 1;
    for (int i = 0; i < 6; i++) {  // max 6 rows
        std::cout << text_code << v_line;
        for (int j = 0; j < 7; j++) {  // 7 days in a week
            if ((i == 0 && j < first_day) || day_counter > days) {
                std::cout << "   ";
            } else {
                std::cout << std::setw(2) << day_counter << " ";
                day_counter++;
            }
        }
        std::cout << v_line << reset_code << std::endl;
        
        // Break if we've printed all days
        if (day_counter > days) break;
    }
    
    std::cout << text_code << bl_corner << h_line << repeat_string(h_line, 20) << br_corner << reset_code << std::endl;
}

// Function to showcase all drawbox features
void showcase_features()
{
    draw_banner("=== DRAWBOX FEATURE SHOWCASE ===", "bg_blue", "bold_white");
    std::cout << std::endl;

    // Basic box (hollow)
    std::cout << "1. Basic Box (hollow):" << std::endl;
    draw_box("Hello, World!");
    std::cout << std::endl;

    // Solid box with custom colors
    std::cout << "2. Solid Box with custom colors:" << std::endl;
    draw_box("Hello, World!", true, "bg_green", "bold_white");
    std::cout << std::endl;

    // Basic table
    std::cout << "3. Basic Table:" << std::endl;
    std::vector<std::vector<std::string>> basic_table = {
        {"Name", "Age", "Role"},
        {"John Doe", "32", "Developer"},
        {"Jane Smith", "28", "Designer"},
        {"Bob Johnson", "45", "Manager"}};
    draw_table(basic_table);
    std::cout << std::endl;

    // Solid table with custom colors
    std::cout << "4. Solid Table with custom colors:" << std::endl;
    std::vector<std::vector<std::string>> solid_table = {
        {"Product", "Price", "Stock"},
        {"Laptop", "$1200", "25"},
        {"Phone", "$800", "45"},
        {"Tablet", "$500", "15"}};
    draw_table(solid_table, true, false, "bg_magenta", "bold_white");
    std::cout << std::endl;

    // Hollow table with colored borders
    std::cout << "5. Hollow Table with colored borders:" << std::endl;
    std::vector<std::vector<std::string>> hollow_table = {
        {"Quarter", "Revenue", "Profit"},
        {"Q1", "$250K", "$45K"},
        {"Q2", "$310K", "$62K"},
        {"Q3", "$285K", "$58K"},
        {"Q4", "$340K", "$75K"}};
    draw_table(hollow_table, false, true, "bg_blue", "bold_cyan");
    std::cout << std::endl;

    // Banner
    std::cout << "6. Banner:" << std::endl;
    draw_banner("Welcome to DrawBox!", "bg_yellow", "black");
    std::cout << std::endl;

    // Textbox demo (simulation)
    std::cout << "7. Textbox (normally interactive - example only):" << std::endl;
    std::cout << "   Example of how it would appear:" << std::endl;
    std::cout << "   \033[44m\033[1;37m Enter your name: \033[0m\033[34m \033[1;37m" << std::endl;
    std::cout << "   > Alice" << std::endl;
    std::cout << std::endl;

    // Progress bar demo
    std::cout << "8. Progress Bar:" << std::endl;
    draw_progress_bar(50, 100, 30, "block_full", "block_light", "bold_white");
    draw_progress_bar(75, 100, 30, "block_light", "block_dark", "bold_green");
    std::cout << std::endl;

    // Tooltip demo
    std::cout << "9. Tooltip Examples:" << std::endl;
    draw_tooltip("This is a tooltip", "info", false, "bold_white");
    draw_tooltip("Warning: System overload", "warning", true, "bold_yellow");
    std::cout << std::endl;

    // Calendar demo
    std::cout << "10. Calendar Example:" << std::endl;
    draw_calendar(5, 2025, "bold_white");
    std::cout << std::endl;

    // Unicode character demo
    std::cout << "11. Unicode Characters:" << std::endl;
    std::cout << "   Checkmark: " << get_unicode("check") << std::endl;
    std::cout << "   Cross mark: " << get_unicode("cross_mark") << std::endl;
    std::cout << "   Smiley face: " << get_unicode("smile") << std::endl;
    std::cout << std::endl;


    // Show color combinations
    std::cout << "8. Color Combinations Examples:" << std::endl;
    draw_box("Red on Black", true, "bg_black", "bold_red");
    draw_box("Yellow on Blue", true, "bg_blue", "bold_yellow");
    draw_box("Green on Magenta", true, "bg_magenta", "bold_green");
    draw_box("Bold White on Red", true, "bg_red", "bold_white");
    draw_box("Cyan on Green", true, "bg_green", "bold_cyan");
    std::cout << std::endl;

    // Available colors section
    std::cout << "9. Available Colors:" << std::endl;
    std::cout << "   Foreground colors: ";
    for (const auto &color_pair : COLOR_MAP)
    {
        if (color_pair.first.substr(0, 3) != "bg_" && color_pair.first.substr(0, 5) != "bold_")
        {
            std::cout << color_pair.second << color_pair.first << "\033[0m ";
        }
    }
    std::cout << std::endl;

    std::cout << "   Bold colors: ";
    for (const auto &color_pair : COLOR_MAP)
    {
        if (color_pair.first.substr(0, 5) == "bold_")
        {
            std::cout << color_pair.second << color_pair.first.substr(5) << "\033[0m ";
        }
    }
    std::cout << std::endl;

    std::cout << "   Background colors: ";
    for (const auto &color_pair : COLOR_MAP)
    {
        if (color_pair.first.substr(0, 3) == "bg_")
        {
            std::cout << color_pair.second << " " << color_pair.first.substr(3) << " \033[0m ";
        }
    }
    std::cout << std::endl
              << std::endl;

    draw_banner("=== END OF SHOWCASE ===", "bg_blue", "bold_white");
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    // Set locale for proper Unicode support
    setlocale(LC_ALL, "");

    if (argc < 2)
    {
        std::cerr << "Error: Missing required argument.\n";
        display_help();
        return 1;
    }

    std::string command = argv[1];

    // Handle help option
    if (command == "--help")
    {
        display_help();
        return 0;
    }

    // Handle showcase option
    if (command == "showcase")
    {
        showcase_features();
        return 0;
    }

    // Check if the first argument is a mode or the text for the default box
    bool is_mode = (command == "box" || command == "table" || command == "banner" || command == "textbox" || command == "progress" || command == "art" || command == "tooltip" || command == "calendar" || command == "unicode" || command == "list_unicode");

    if (!is_mode)
    {
        // If not a mode, assume it's text for the default box mode
        std::string text = command;
        bool solid = false;
        std::string bg_color = "bg_blue";
        std::string text_color = "bold_white";

        for (int i = 2; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "solid")
            {
                solid = true;
            }
            else if (arg.substr(0, 3) == "bg_")
            {
                if (!is_valid_color(arg.substr(3), true))
                {
                    std::cerr << "Error: Invalid background color '" << arg << "'.\n";
                    display_help();
                    return 1;
                }
                bg_color = arg;
            }
            else
            {
                if (!is_valid_color(arg))
                {
                    std::cerr << "Error: Invalid text color '" << arg << "'.\n";
                    display_help();
                    return 1;
                }
                text_color = arg;
            }
        }
        draw_box(text, solid, bg_color, text_color);
        return 0;
    }

    // Handle box mode
    if (command == "box")
    {
        if (argc < 3)
        {
            std::cerr << "Error: Missing required argument for box text.\n";
            display_help();
            return 1;
        }
        std::string text = argv[2];
        bool solid = false;
        std::string bg_color = "bg_blue";
        std::string text_color = "bold_white";

        for (int i = 3; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "solid")
            {
                solid = true;
            }
            else if (arg.substr(0, 3) == "bg_")
            {
                if (!is_valid_color(arg.substr(3), true))
                {
                    std::cerr << "Error: Invalid background color '" << arg << "'.\n";
                    display_help();
                    return 1;
                }
                bg_color = arg;
            }
            else
            {
                if (!is_valid_color(arg))
                {
                    std::cerr << "Error: Invalid text color '" << arg << "'.\n";
                    display_help();
                    return 1;
                }
                text_color = arg;
            }
        }
        draw_box(text, solid, bg_color, text_color);
    }

    // Handle table mode
    else if (command == "table")
    {
        if (argc < 3)
        {
            std::cerr << "Error: Missing rows for table.\n";
            display_help();
            return 1;
        }
        std::vector<std::vector<std::string>> rows;
        int arg_index = 2;

        // Parse rows
        while (arg_index < argc)
        {
            std::string arg = argv[arg_index];

            // Break if we encounter a style or color argument
            if (arg == "solid" || arg == "hollow" || arg.substr(0, 3) == "bg_" ||
                COLOR_MAP.count(arg) > 0)
            {
                break;
            }

            std::vector<std::string> row;
            std::istringstream row_stream(arg);
            std::string cell;
            while (std::getline(row_stream, cell, ','))
            {
                row.push_back(cell);
            }
            rows.push_back(row);
            arg_index++;
        }

        // Default is no color mode
        bool solid_mode = false;
        bool hollow_mode = false;
        std::string bg_color = "bg_blue";
        std::string text_color = "bold_white";

        // Parse style and color options
        while (arg_index < argc)
        {
            std::string arg = argv[arg_index];
            if (arg == "solid")
            {
                solid_mode = true;
                hollow_mode = false;
            }
            else if (arg == "hollow")
            {
                hollow_mode = true;
                solid_mode = false;
            }
            else if (arg.substr(0, 3) == "bg_")
            {
                if (!is_valid_color(arg.substr(3), true))
                {
                    std::cerr << "Error: Invalid background color '" << arg << "'.\n";
                    display_help();
                    return 1;
                }
                bg_color = arg;
            }
            else
            {
                if (!is_valid_color(arg))
                {
                    std::cerr << "Error: Invalid text color '" << arg << "'.\n";
                    display_help();
                    return 1;
                }
                text_color = arg;
            }
            arg_index++;
        }

        // Draw the table based on mode
        draw_table(rows, solid_mode, hollow_mode, bg_color, text_color);
    }

    // Handle banner mode
    else if (command == "banner")
    {
        if (argc < 3)
        {
            std::cerr << "Error: Missing text for banner.\n";
            display_help();
            return 1;
        }
        std::string text = argv[2];
        std::string bg_color = "bg_blue";
        std::string text_color = "bold_white";

        for (int i = 3; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.substr(0, 3) == "bg_")
            {
                if (!is_valid_color(arg.substr(3), true))
                {
                    std::cerr << "Error: Invalid background color '" << arg << "'.\n";
                    display_help();
                    return 1;
                }
                bg_color = arg;
            }
            else
            {
                if (!is_valid_color(arg))
                {
                    std::cerr << "Error: Invalid text color '" << arg << "'.\n";
                    display_help();
                    return 1;
                }
                text_color = arg;
            }
        }
        draw_banner(text, bg_color, text_color);
    }

    // Handle textbox mode
    else if (command == "textbox")
    {
        if (argc < 3)
        {
            std::cerr << "Error: Missing title for textbox.\n";
            display_help();
            return 1;
        }

        std::string title = argv[2];
        std::string bg_color = "bg_blue";      // Default background color
        std::string text_color = "bold_white"; // Default text color

        // Process additional arguments for colors
        for (int i = 3; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.substr(0, 3) == "bg_")
            {
                if (!is_valid_color(arg.substr(3), true))
                {
                    std::cerr << "Error: Invalid background color '" << arg << "'.\n";
                    display_help();
                    return 1;
                }
                bg_color = arg; // Set the background color
            }
            else
            {
                if (!is_valid_color(arg))
                {
                    std::cerr << "Error: Invalid color '" << arg << "'.\n";
                    display_help();
                    return 1;
                }
                text_color = arg; // Set the text color
            }
        }

        // Call the draw_textbox function with the arguments
        draw_textbox(title, bg_color, text_color);

    } else if (command == "progress")
    {
        if (argc < 4)
        {
            std::cerr << "Error: Missing required arguments for progress bar.\n";
            display_help();
            return 1;
        }
        
        int value = std::stoi(argv[2]);
        int max = std::stoi(argv[3]);
        int width = (argc > 4) ? std::stoi(argv[4]) : 20;
        std::string fill_char = (argc > 5) ? argv[5] : "block_full";
        std::string empty_char = (argc > 6) ? argv[6] : "block_light";
        std::string text_color = (argc > 7) ? argv[7] : "bold_white";
        
        draw_progress_bar(value, max, width, fill_char, empty_char, text_color);
    }
    else if (command == "art")
    {
        if (argc < 3)
        {
            std::cerr << "Error: Missing file path for art.\n";
            display_help();
            return 1;
        }
        
        std::string file_path = argv[2];
        std::ifstream file(file_path);
        
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open file '" << file_path << "'.\n";
            return 1;
        }
        
        std::vector<std::string> art_rows;
        std::string line;
        
        while (std::getline(file, line)) {
            art_rows.push_back(line);
        }
        
        std::string text_color = (argc > 3) ? argv[3] : "bold_white";
        draw_unicode_art(art_rows, text_color);
    }
    else if (command == "tooltip")
    {
        if (argc < 3)
        {
            std::cerr << "Error: Missing text for tooltip.\n";
            display_help();
            return 1;
        }
        
        std::string text = argv[2];
        std::string icon = (argc > 3) ? argv[3] : "info";
        bool use_double = (argc > 4 && std::string(argv[4]) == "double");
        std::string text_color = (argc > 5) ? argv[5] : "bold_white";
        
        draw_tooltip(text, icon, use_double, text_color);
    }
    else if (command == "calendar")
    {
        if (argc < 4)
        {
            std::cerr << "Error: Missing month or year for calendar.\n";
            display_help();
            return 1;
        }
        
        int month = std::stoi(argv[2]);
        int year = std::stoi(argv[3]);
        std::string text_color = (argc > 4) ? argv[4] : "bold_white";
        
        if (month < 1 || month > 12)
        {
            std::cerr << "Error: Month must be between 1 and 12.\n";
            return 1;
        }
        
        draw_calendar(month, year, text_color);
    }
    else if (command == "unicode")
    {
        if (argc < 3)
        {
            std::cerr << "Error: Missing character name.\n";
            display_help();
            return 1;
        }
        
        std::string name = argv[2];
        std::cout << get_unicode(name) << std::endl;
    }
    else if (command == "list_unicode")
    {
        std::string category = (argc > 2) ? argv[2] : "all";
        
        if (category == "all" || category == "box")
        {
            std::cout << "Box Drawing Characters:" << std::endl;
            for (const auto &pair : unicode::BOX_CHARS)
            {
                std::cout << pair.first << " : " << pair.second << "  ";
                if (pair.first.length() < 10) std::cout << "\t";
                if ((std::distance(unicode::BOX_CHARS.begin(), unicode::BOX_CHARS.find(pair.first)) + 1) % 5 == 0)
                    std::cout << std::endl;
            }
            std::cout << std::endl << std::endl;
        }
        
        if (category == "all" || category == "emoji")
        {
            std::cout << "Emoji Characters:" << std::endl;
            for (const auto &pair : unicode::EMOJI)
            {
                std::cout << pair.first << " : " << pair.second << "  ";
                if (pair.first.length() < 10) std::cout << "\t";
                if ((std::distance(unicode::EMOJI.begin(), unicode::EMOJI.find(pair.first)) + 1) % 5 == 0)
                    std::cout << std::endl;
            }
            std::cout << std::endl << std::endl;
        }
        
        if (category == "all" || category == "ui")
        {
            std::cout << "UI Icon Characters:" << std::endl;
            for (const auto &pair : unicode::UI_ICONS)
            {
                std::cout << pair.first << " : " << pair.second << "  ";
                if (pair.first.length() < 10) std::cout << "\t";
                if ((std::distance(unicode::UI_ICONS.begin(), unicode::UI_ICONS.find(pair.first)) + 1) % 5 == 0)
                    std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }

    else
    {
        std::cerr << "Error: Unknown command '" << command << "'.\n";
        display_help();
        return 1;
    }

    return 0;
}
