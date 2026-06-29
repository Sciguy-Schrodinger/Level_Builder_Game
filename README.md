# 🏗️ Level Builder Game

A level-building game where players can create and play custom levels. Built with **C++** and **Raylib**.

---

## 🎮 Features

- 🖱️ Click and drag to place limited number of platforms in a clever way
- ▶️ Playtest your levels immediately
- ↪️ Revise and playtest your levels to achieve the goal 
- 🎨 Simple grid-based snapping
- 🔊 Sound effects and music

---

## 🛠️ Technologies Used

- **C++** — Core game logic
- **Raylib** — 2D rendering, input handling, audio

___

### Requirements

- Raylib
- GCC or Clang

___

### 🚀 How to Build

```bash
g++ -o level_builder \
    main.cpp game_maker.cpp main_menu.cpp \
    -lraylib -lm -lpthread -ldl -lrt -lX11
