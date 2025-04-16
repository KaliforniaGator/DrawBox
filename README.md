![Screen Shot 2025-04-15 at 9(1)](https://github.com/user-attachments/assets/efec00a7-423e-4a97-8544-40f6d6eeaaa5)

# DrawBox

DrawBox is a versatile command-line utility that enhances terminal output with beautifully formatted boxes, tables, banners, and interactive textboxes. It supports customizable background and text colors, with multiple display modes for different visual needs.

## Features

- **Multiple Display Modes**:
  - **Box**: Create simple or solid boxes around text
  - **Table**: Format data in organized tables with customizable styles
  - **Banner**: Display attention-grabbing banners for important messages
  - **Textbox**: Create interactive input prompts with styled labels

- **Styling Options**:
  - **Solid/Hollow Modes**: Choose filled backgrounds or colored outlines
  - **Color Customization**: Extensive color palette for backgrounds and text
  - **Unicode Characters**: Beautiful box-drawing characters for clean visuals

- **Convenience Features**:
  - **Showcase**: Preview all DrawBox capabilities with a single command
  - **Help**: Detailed usage instructions with examples
  - **Input Capture**: Redirect textbox input to files or other programs

## Installation

### Quick Installation (Recommended)

Use the installation script to automatically download, compile, and install DrawBox:

```bash
curl -s https://raw.githubusercontent.com/KaliforniaGator/DrawBox/main/update.sh | bash
```

This script will:
1. Download the latest DrawBox source code
2. Install the necessary compiler if not present
3. Compile the source code
4. Move the binary to `/usr/local/bin/` for system-wide access

### From Source

If you prefer to install manually:

1. Clone the repository:
   ```bash
   git clone https://github.com/kaliforniagator/drawbox.git
   ```

2. Navigate to the project directory:
   ```bash
   cd drawbox
   ```

3. Compile the source code:
   ```bash
   g++ drawbox.cpp -o drawbox
   ```
   On MacOS compile with the following command:
   ```bash
   clang++ -std=c++11 -o drawbox drawbox.cpp
   ```
   This will use a later version of C++ to ensure the binary compiles.
   
4. (Optional) Move the binary to a directory in your `PATH`:
   ```bash
   sudo mv drawbox /usr/local/bin/
   ```

## Usage

```bash
drawbox [mode] <arguments>...
```

### Box Mode (default)
```bash
drawbox [box] <text> [solid] [bg_color] [text_color]
```

### Table Mode
```bash
drawbox table <row1> <row2> ... [solid|hollow] [bg_color] [text_color]
```

### Banner Mode
```bash
drawbox banner <text> [bg_color] [text_color]
```

### Textbox Mode
```bash
drawbox textbox <title> [bg_color] [text_color]
```

### Other Commands
```bash
drawbox showcase    # Display examples of all features
drawbox --help      # Show usage instructions
```

## Examples

1. Simple hollow box with default colors:
   ```bash
   drawbox "Hello, World!"
   ```

2. Solid box with custom colors:
   ```bash
   drawbox "Hello, World!" solid bg_green bold_white
   ```

3. Create a table with multiple rows:
   ```bash
   drawbox table "Name,Age,Role" "John,32,Developer" "Alice,28,Designer"
   ```

4. Create a colored table:
   ```bash
   drawbox table "Name,Age,Role" "John,32,Developer" "Alice,28,Designer" solid bg_magenta bold_white
   ```

5. Display an attention-grabbing banner:
   ```bash
   drawbox banner "Welcome to My Program!" bg_yellow bold_black
   ```

6. Interactive input with styled prompt:
   ```bash
   drawbox textbox "Enter your name:" bg_cyan bold_white
   ```

7. Capture input to a file:
   ```bash
   drawbox textbox "Enter your name:" > username.txt
   ```

## Supported Colors

### Background Colors
- `bg_black`, `bg_red`, `bg_green`, `bg_yellow`
- `bg_blue`, `bg_magenta`, `bg_cyan`, `bg_white`

### Text Colors
- Regular: `black`, `red`, `green`, `yellow`, `blue`, `magenta`, `cyan`, `white`
- Bold: `bold_black`, `bold_red`, `bold_green`, `bold_yellow`, `bold_blue`, `bold_magenta`, `bold_cyan`, `bold_white`

## Contributing

Contributions are welcome! Please open an issue or submit a pull request for any improvements or bug fixes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
