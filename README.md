# 🏗️ Level Builder Game

A level-building game where players can create, save, and play custom levels. Built with **C++** and **Raylib**.

---

## 🎮 Features

- 🖱️ Click and drag to place platforms, enemies, coins, and exits
- 🗺️ Save and load custom levels
- ▶️ Playtest your levels immediately
- 🎨 Simple grid-based snapping
- 🔊 Sound effects and music

---

## 🛠️ Technologies Used

- **C++** — Core game logic
- **Raylib** — 2D rendering, input handling, audio

---

## 📁 Project Structure

---

## 🚀 How to Build

### Requirements

- Raylib
- GCC or Clang

### Build

```bash
g++ -o level_builder \
    main.cpp game_maker.cpp main_menu.cpp \
    -lraylib -lm -lpthread -ldl -lrt -lX11
