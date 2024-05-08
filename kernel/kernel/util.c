void mem_copy(char* source, char* destination, int nbytes)
{
    for (int i = 0; i < nbytes; i++)
    {
        *(destination + i) = *(source + i);
    }
}

int strlen(char str[])
{
    int i = 0;

    while (str[i] != '\0')
        ++i;

    return i;
}

// for printing numbers in correct order
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
}
