#ifndef STRING_H
#define STRING_H

#include <stdint.h>

int strlen(char str[]);
void reverse(char str[]);
void int_to_ascii(int n, char str[]);
void backspace(char str[]);
void append(char str[], char c);
int strcmp(char str1[], char str2[]);
char char_upper(char c);
void hex_to_ascii(int n, char str[]);

#endif
