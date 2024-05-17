#ifndef SCREEN_H
#define SCREEN_H

#include "screen_colors.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"
#include <stdint.h>

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

// screen i/o ports
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

// kernel api
void clear_screen();
void kprint_at(char* message, int col, int row);
void kprint(char* message);
void kprint_color_at(char* message, int col, int row, char color);
void kprint_color(char* message, char color);
void kprint_backspace();

#endif
