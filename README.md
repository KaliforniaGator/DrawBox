# DrawBox

DrawBox is a simple command-line tool that prints a box with text onto the shell. It supports customizable background and text colors, and can create both solid and hollow boxes.
![Screen Shot 2025-02-24 at 5 49 47 PM](https://github.com/user-attachments/assets/11ac9114-f86b-4d7b-9c61-29463f04b6b1)

## Features

- **Customizable Text**: Display any text inside the box.
- **Solid or Hollow Boxes**: Choose between solid or hollow box styles.
- **Color Customization**: Set custom background and text colors using ANSI color codes.
- **Easy to Use**: Simple command-line interface with optional arguments.
- **List of Colors**: Use the `-l` or `--list` commands to see all available colors.
- **Help**: Use the `--help` command to see information about DrawBox

## Installation

### From Source

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
   
5. (Optional) Move the binary to a directory in your `PATH`:
   ```bash
   sudo mv drawbox /usr/local/bin/
   ```

### Using Precompiled Binary

**WARNING PRE-COMPILED BINARIES MAY NOT BE UP TO DATE, IT IS ADVISED THAT YOU COMPILE IT YOURSELF**

1. Download the binary from the [Releases](https://github.com/kaliforniagator/drawbox/releases) page.
2. (Optional) Move the binary to a directory in your `PATH`:
   ```bash
   sudo mv drawbox /usr/local/bin/
   ```

## Usage

```bash
drawbox <text> [solid] [bg_color] [text_color]
```
On MacOS use this format:
```bash
drawbox 'text' [solid] [bg_color] [text_color]
```

- `<text>`: The text to display in the box (required).
- `[solid]`: Optional. Use `solid` to create a solid box.
- `[bg_color]`: Optional. Background color (e.g., `bg_blue`, `bg_red`).
- `[text_color]`: Optional. Text color (e.g., `white`, `green`).

### Examples

1. Display a hollow box with default colors:
   ```bash
   drawbox "Hello, World!"
   ```

2. Display a solid box with a green background and white text:
   ```bash
   drawbox "Hello, World!" solid bg_green white
   ```

3. Display a hollow box with red text:
   ```bash
   drawbox "Hello, World!" red
   ```

## Supported Colors

### Background Colors

- `bg_black`
- `bg_red`
- `bg_green`
- `bg_yellow`
- `bg_blue`
- `bg_magenta`
- `bg_cyan`
- `bg_white`

### Text Colors

- `black`
- `red`
- `green`
- `yellow`
- `blue`
- `magenta`
- `cyan`
- `white`
- `bold_black`
- `bold_red`
- `bold_green`
- `bold_yellow`
- `bold_blue`
- `bold_magenta`
- `bold_cyan`
- `bold_white`

## Contributing

Contributions are welcome! Please open an issue or submit a pull request for any improvements or bug fixes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
