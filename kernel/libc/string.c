#include "string.h"
#include "../cpu/types.h"

// get the length of the string
int strlen(char str[])
{
    int i = 0;

    while (str[i] != '\0')
        ++i;

    return i;
}

// reverse the string order (for printing numbers in correct order)
void reverse(char str[])
{
    int c, i, j;

    for (i = 0, j = strlen(str) - 1; i < j; i++, j--)
    {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

// k&r implementation
void int_to_ascii(int n, char str[])
{
    int i = 0;
    int sign;

    if ((sign = n) < 0) 
        n = -n;

    do 
    {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0)
        str[i++] = '-';

    str[i] = '\0';

    reverse(str);
}

// handling backspace
void backspace(char str[])
{
    int len = strlen(str);
    str[len - 1] = '\0';
}

// append 1 character to string
void append(char str[], char c)
{
    int len = strlen(str);
    str[len] = c;
    str[len + 1] = '\0';
}

// returns < 0 if str1 < str2, 0 if str1 == str2, > 0 if str1 > str2
int strcmp(char str1[], char str2[])
{
    int i;

    for (i = 0; str1[i] == str2[i]; i++)
        if (str1[i] == '\0')
            return 0;

    return str1[i] - str2[i];
}

char char_upper(char c)
{
    if (c > 96 && c < 123) // if is valid lowercase character
        c ^= 0x20; // toggles 5th bit in ascii value

    return c;
}

void hex_to_ascii(int n, char str[])
{
    append(str, '0');
    append(str, 'x');
    char zeros = 0;

    s32 temp;
    for (int i = 28; i > 0; i -= 4)
    {
        temp = (n >> i) & 0xF;

        if (temp == 0 && zeros == 0)
            continue;

        zeros = 1;

        if (temp > 0xA)
            append(str, temp - 0xA + 'a');
        else
            append(str, temp + '0');
    }

    temp = n & 0xF;
    if (temp >= 0xA)
        append(str, temp - 0xA + 'a');
    else
        append(str, temp + '0');
}
