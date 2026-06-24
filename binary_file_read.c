#include <stdio.h>
#include <stdint.h>

int main()
{
    FILE *f = fopen("sorted.bin", "rb");
    if (!f) return 1;

    uint32_t x;

    while (fread(&x, sizeof(uint32_t), 1, f) == 1)
    {
        printf("%u\n", x);
    }

    fclose(f);
    return 0;
}