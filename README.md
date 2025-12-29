# 🎮 C++ Word Hunt (Kelime Avı)

Advanced console-based Word Hunt / Hangman game developed using C++. This project demonstrates Object-Oriented Programming (OOP) principles, file handling, and custom console UI design.

![C++](https://img.shields.io/badge/C++-Solutions-blue.svg?style=flat&logo=c%2B%2B)
![Status](https://img.shields.io/badge/Status-Finished-green.svg)

## Features

* ** Profile System:** Saves player stats (wins, games played, total score) to a file.
* ** File Handling:** Reads words dynamically from `words.txt`. Includes a **Backup System** if the file is missing.
* ** Custom UI:** Colorful console interface, ASCII art animations, and fixed window size.
* ** Smart Hint System:** AI-like hint logic that reveals a random letter along with a text clue.
* ** Time Challenge:** 60-second timer for each move.
* ** Scoring Logic:** Dynamic scoring based on difficulty level and remaining lives.

##  How to Run

1.  Ensure you have a C++ compiler (g++).
2.  Clone the repository or download `main.cpp`.
3.  Compile the code:
    ```bash
    g++ main.cpp -o game
    ```
4.  Run the executable:
    ```bash
    ./game
    ```

> **Note:** This project uses the `windows.h` library for console manipulation (colors, window size). Therefore, it is designed to run natively on **Windows** operating systems.

##  Screenshots

The game features a custom "Application-like" console window:

```text
+------------------------------------------+
|            KELIME AVI PRO                |
+------------------------------------------+
|  [1] MEYVE                               |
|  [2] SEHIR                               |
|  [3] HAYVAN                              |
+------------------------------------------+


Developed by [Özgür Çağan]

