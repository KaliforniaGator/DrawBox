#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>

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
        std::cout << bg_code  << text_code << "│" << std::string(left_spaces, ' ') << text_code << text << bg_code << text_code << std::string(right_spaces, ' ') << "│" << reset_code << "\n";
        std::cout << bg_code << text_code << "└" << horizontal_line << "┘" << reset_code << "\n";
    }
    else
    {
        std::cout << text_code << "┌" << horizontal_line << "┐" << reset_code << "\n";
        std::cout << text_code << "│" << std::string(left_spaces, ' ') << text << std::string(right_spaces, ' ') << "│" << reset_code << "\n";
        std::cout << text_code << "└" << horizontal_line << "┘" << reset_code << "\n";
    }
}

// Function to draw a table
void draw_table(const std::vector<std::vector<std::string>> &rows, bool solid_mode = false,
                bool hollow_mode = false, const std::string &bg_color = "bg_blue",
                const std::string &text_color = "bold_white")
{
    std::string bg_code = "";
    std::string text_code = "";
    std::string border_color_code = "";
    std::string reset_code = "\033[0m";

    // Set up color codes based on mode
    if (solid_mode)
    {
        // Solid mode - use background color and text color
        bg_code = COLOR_MAP.count(bg_color) ? COLOR_MAP.at(bg_color) : COLOR_MAP.at("bg_blue");
        text_code = COLOR_MAP.count(text_color) ? COLOR_MAP.at(text_color) : COLOR_MAP.at("bold_white");
    }
    else if (hollow_mode)
    {
        // Hollow mode - only use text color for border characters
        border_color_code = COLOR_MAP.count(text_color) ? COLOR_MAP.at(text_color) : COLOR_MAP.at("bold_white");
    }
    // Otherwise, no color mode - all codes remain empty

    // Find the maximum length of the columns
    std::vector<int> column_widths;
    for (const auto &row : rows)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            if (column_widths.size() <= i)
            {
                column_widths.push_back(row[i].length());
            }
            else
            {
                column_widths[i] = std::max(column_widths[i], static_cast<int>(row[i].length()));
            }
        }
    }

    // Unicode box drawing characters
    const char *top_left = "┌";
    const char *top_right = "┐";
    const char *bottom_left = "└";
    const char *bottom_right = "┘";
    const char *horizontal = "─";
    const char *vertical = "│";
    const char *top_t = "┬";
    const char *bottom_t = "┴";
    const char *left_t = "├";
    const char *right_t = "┤";
    const char *cross = "┼";

    // Draw top border
    if (solid_mode)
    {
        // Solid mode with background color
        std::cout << bg_code << text_code << top_left;
        for (size_t i = 0; i < column_widths.size(); ++i)
        {
            std::cout << repeat_string(horizontal, column_widths[i] + 2);
            if (i < column_widths.size() - 1)
            {
                std::cout << top_t;
            }
        }
        std::cout << top_right << reset_code << "\n";
    }
    else if (hollow_mode)
    {
        // Hollow mode with colored borders
        std::cout << border_color_code << top_left;
        for (size_t i = 0; i < column_widths.size(); ++i)
        {
            std::cout << repeat_string(horizontal, column_widths[i] + 2);
            if (i < column_widths.size() - 1)
            {
                std::cout << top_t;
            }
        }
        std::cout << top_right << reset_code << "\n";
    }
    else
    {
        // No color mode
        std::cout << top_left;
        for (size_t i = 0; i < column_widths.size(); ++i)
        {
            std::cout << repeat_string(horizontal, column_widths[i] + 2);
            if (i < column_widths.size() - 1)
            {
                std::cout << top_t;
            }
        }
        std::cout << top_right << "\n";
    }

    // Draw rows
    for (const auto &row : rows)
    {
        // Draw row content
        if (solid_mode)
        {
            // Solid mode with background color
            std::cout << bg_code << text_code << vertical;
            for (size_t i = 0; i < row.size(); ++i)
            {
                std::cout << " " << std::setw(column_widths[i]) << row[i] << " ";
                if (i < row.size() - 1)
                {
                    std::cout << vertical;
                }
                else
                {
                    std::cout << vertical << reset_code;
                }
            }
            std::cout << "\n";
        }
        else if (hollow_mode)
        {
            // Hollow mode with colored borders
            std::cout << border_color_code << vertical << reset_code;
            for (size_t i = 0; i < row.size(); ++i)
            {
                std::cout << " " << std::setw(column_widths[i]) << row[i] << " ";
                if (i < row.size() - 1)
                {
                    std::cout << border_color_code << vertical << reset_code;
                }
                else
                {
                    std::cout << border_color_code << vertical << reset_code;
                }
            }
            std::cout << "\n";
        }
        else
        {
            // No color mode
            std::cout << vertical;
            for (size_t i = 0; i < row.size(); ++i)
            {
                std::cout << " " << std::setw(column_widths[i]) << row[i] << " ";
                if (i < row.size() - 1)
                {
                    std::cout << vertical;
                }
                else
                {
                    std::cout << vertical;
                }
            }
            std::cout << "\n";
        }

        // Draw horizontal separator after each row except the last
        if (rows.size() > 1 && &row != &rows.back())
        {
            if (solid_mode)
            {
                // Solid mode with background color
                std::cout << bg_code << text_code << left_t;
                for (size_t i = 0; i < column_widths.size(); ++i)
                {
                    std::cout << repeat_string(horizontal, column_widths[i] + 2);
                    if (i < column_widths.size() - 1)
                    {
                        std::cout << cross;
                    }
                }
                std::cout << right_t << reset_code << "\n";
            }
            else if (hollow_mode)
            {
                // Hollow mode with colored borders
                std::cout << border_color_code << left_t;
                for (size_t i = 0; i < column_widths.size(); ++i)
                {
                    std::cout << repeat_string(horizontal, column_widths[i] + 2);
                    if (i < column_widths.size() - 1)
                    {
                        std::cout << cross;
                    }
                }
                std::cout << right_t << reset_code << "\n";
            }
            else
            {
                // No color mode
                std::cout << left_t;
                for (size_t i = 0; i < column_widths.size(); ++i)
                {
                    std::cout << repeat_string(horizontal, column_widths[i] + 2);
                    if (i < column_widths.size() - 1)
                    {
                        std::cout << cross;
                    }
                }
                std::cout << right_t << "\n";
            }
        }
    }

    // Draw bottom border
    if (solid_mode)
    {
        // Solid mode with background color
        std::cout << bg_code << text_code << bottom_left;
        for (size_t i = 0; i < column_widths.size(); ++i)
        {
            std::cout << repeat_string(horizontal, column_widths[i] + 2);
            if (i < column_widths.size() - 1)
            {
                std::cout << bottom_t;
            }
        }
        std::cout << bottom_right << reset_code << "\n";
    }
    else if (hollow_mode)
    {
        // Hollow mode with colored borders
        std::cout << border_color_code << bottom_left;
        for (size_t i = 0; i < column_widths.size(); ++i)
        {
            std::cout << repeat_string(horizontal, column_widths[i] + 2);
            if (i < column_widths.size() - 1)
            {
                std::cout << bottom_t;
            }
        }
        std::cout << bottom_right << reset_code << "\n";
    }
    else
    {
        // No color mode
        std::cout << bottom_left;
        for (size_t i = 0; i < column_widths.size(); ++i)
        {
            std::cout << repeat_string(horizontal, column_widths[i] + 2);
            if (i < column_widths.size() - 1)
            {
                std::cout << bottom_t;
            }
        }
        std::cout << bottom_right << "\n";
    }
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

// Function to showcase all drawbox features
void showcase_features()
{
    std::cout << "\n=== DRAWBOX FEATURE SHOWCASE ===\n"
              << std::endl;

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
    draw_banner("Welcome to DrawBox!", "bg_yellow", "bold_black");
    std::cout << std::endl;

    // Textbox demo (simulation)
    std::cout << "7. Textbox (normally interactive - example only):" << std::endl;
    std::cout << "   Example of how it would appear:" << std::endl;
    std::cout << "   \033[44m\033[1;37m Enter your name: \033[0m\033[34m \033[1;37m" << std::endl;
    std::cout << "   > Alice" << std::endl;
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

    std::cout << "=== END OF SHOWCASE ===\n"
              << std::endl;
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
    std::cerr << "\nExamples:\n";
    std::cerr << "  drawbox \"Hello, World!\" solid bg_green white\n";
    std::cerr << "  drawbox table \"Name,Age\" \"John,25\" \"Alice,30\"\n";
    std::cerr << "  drawbox table \"Name,Age\" \"John,25\" \"Alice,30\" color bg_cyan white\n";
    std::cerr << "  drawbox banner \"Welcome to My Program!\" bg_magenta white\n";
    std::cerr << "  drawbox textbox \"Enter your name:\" bg_cyan white\n";
    std::cerr << "  drawbox textbox \"Title:\" > hello.txt\n";
}

int main(int argc, char *argv[])
{
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
    bool is_mode = (command == "box" || command == "table" || command == "banner" || command == "textbox");

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
    }

    else
    {
        std::cerr << "Error: Unknown command '" << command << "'.\n";
        display_help();
        return 1;
    }

    return 0;
}
