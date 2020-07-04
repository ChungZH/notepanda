# Changelog

All notable changes to this project will be documented in this file.

## UNRELEASED

### 🌵 Features

- Syntax selection menu
- Folding
- Read-Only mode
- Ctrl + Wheel to zoom the text
- Fluent System Icons
- Tab / Spaces indent

## 0.1.3

> If you want to track the latest news of Notepanda, welcome to our [📰 Telegram channel](https://t.me/notepanda)!

### 🌵 Features

- Preview panel
- JSON format settings
- Pin to top
- Set window modified
- New logo
- More available syntax highlight themes: `Breeze Dark`, `Default`, `Printing`, `Solarized Dark` and `Solarized  Light`
- Add normal mode icon
- Sticky mode
    - Hide sidebar
    - Hide line number area
    - Hide statusbar
    - Use general font (not monospace)
    - Smaller window
    - Custom background color

### 🐞 Bug fix

- Fix font settings cannot take effect
- Fix file content lost on save (#30)
- NSIS installer cannot found dll

### ! BREAKING CHANGE !

- The `Style` setting has been renamed to `StyleTheme`
- The `ColorTheme` setting has been renamed to `EditorColorTheme`

A deb package has also been added.

## 0.1.3-alpha2

The main purpose of this release is to fix some bugs in the previous alpha version.

> Are you confused when you see this version number? That's right, we've turned 0.0.3 into the more standardized 0.1.3.

### 🌵 Features

- Pin to top

## 🐞 Bug fix

- NSIS installer cannot found dll

A deb package has also been added.

> If you want to track the latest news of Notepanda, welcome to our [📰 Telegram channel](https://t.me/notepanda)!

## 0.0.3-alpha1

### 🌵 Features

- Set window modified
- New logo
- More available syntax highlight themes: `Breeze Dark`, `Default`, `Printing`, `Solarized Dark` and `Solarized  Light`
- Add normal mode icon
- Sticky mode
    - Hide sidebar
    - Hide line number area
    - Hide statusbar
    - Use general font (not monospace)
    - Smaller window
    - Custom background color

### 🐞 Bug fix

- Fix font settings cannot take effect

## 0.0.2

### 🌵 Features

- Custom font in Preferences Window
- Change the default font to `monospace` (Different on different systems)
- Syntax Highlighting
- Custom theme
- Launch from the shell
- Persistent application settings

### 🦋 Optimization

- Refactor code structure.

## 0.0.1

The following is this version of changelog (including the beta/rc version):

### 🌵 Features

- Basic operation
  - New
  - Open
  - Save
  - Save as
  - Print
  - Undo
  - Redo
  - Copy
  - Paste
  - Cut
  - Quit
- Icons (from [Remix Icon - Open source icon library](https://remixicon.com/))
- Toolbar
- Line numbers
- Highlight current line
- Beautify the interface
- Show characters & line numbers on status bar
- Preferences Window

### 🦋 Optimization

- Refactor
  - Use Lambda to set action undo&redo&copy&cut's state.
  - Move all sources to `src/` dir
  - Move core sources to `src/core` dir, ui sources to `src/ui` dir
  - Rename the `notepanda` class to `MainWindow`

### 🐞 Bug fix

- Fix cannot load SVG files.

## 0.0.1-rc.1

### 🌵 Features

- More basic operation
  - Undo
  - Redo
  - Copy
  - Paste
  - Cut
  - Quit
- Icons (from [Remix Icon - Open source icon library](https://remixicon.com/))
- Toolbar
- Line numbers
- Highlight current line

### 🦋 Optimization

- Refactor
  - Move all sources to `src/` dir
  - Move core sources to `src/core` dir, ui sources to `src/ui` dir
  - Rename the `notepanda` class to `MainWindow`

## 0.0.1-alpha

> First release. :beers:

[GitHub Release](https://github.com/ChungZH/notepanda/releases/tag/v0.0.1-alpha)

### 🌵 Features

- Basic operation
  - New
  - Open
  - Save
  - Save as
