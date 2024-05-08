#include "string.h"

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
void append(char str[], char n)
{
    int len = strlen(str);
    str[len] = n;
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
