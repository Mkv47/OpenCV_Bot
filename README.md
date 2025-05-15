# OpenCV Fishing Bot

A Windows C++ automation bot that uses OpenCV for image recognition and Windows API for mouse/keyboard control to automate fishing tasks in a game.

---

## Features

- Captures the screen and matches template images to detect fishing states
- Automates mouse clicks and key presses to simulate fishing actions
- Detects when to repair fishing equipment and automates the repair process
- Supports bait switching and salvage actions based on in-game visuals

---

## Requirements

- Windows OS
- OpenCV (C++ version)
- C++17 compatible compiler (e.g., MSVC)
- Windows SDK (for WinAPI functions)

---

## How it Works

The bot repeatedly:

1. Captures the game screen.
2. Compares screenshots with predefined template images (fish, bait, success/failure states).
3. Performs mouse clicks and keyboard inputs to simulate fishing.
4. Repairs fishing gear when durability is low.
5. Changes bait automatically.

---

## Setup

1. Clone the repository:
   ```bash
   git clone https://github.com/Mkv47/OpenCV_Bot.git
