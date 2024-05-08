void mem_copy(char* source, char* destination, int nbytes)
{
    for (int i = 0; i < nbytes; i++)
    {
        *(destination + i) = *(source + i);
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
