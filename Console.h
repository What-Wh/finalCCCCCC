#pragma once
#include <Windows.h>

enum ConsoleColors {
    BLACK = 0,
    DARK_BLUE = FOREGROUND_BLUE,
    DARK_GREEN = FOREGROUND_GREEN,
    DARK_CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
    DARK_RED = FOREGROUND_RED,
    DARK_MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
    DARK_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
    GRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    DARK_GRAY = FOREGROUND_INTENSITY,
    BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
    MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

};