#include <stdio.h>
#include <stdint.h>

int main()
{
    FILE *f = fopen("sorted.txt", "r");
    if (!f) return 1;

    uint32_t x;

    while (fscanf(f, "%u", &x) == 1)
    {
        printf("%u\n", x);
    }

    fclose(f);
    return 0;
}