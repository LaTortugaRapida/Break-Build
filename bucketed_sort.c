#include <stdio.h>
#include <stdint.h>

int main()
{
    FILE *input = fopen("numbers.txt", "r");
    if (!input) return 1;

    FILE *bucket[121];

    for (int i = 0; i <= 120; i++)
    {
        char name[32];
        sprintf(name, "bucket_%d.txt", i);

        bucket[i] = fopen(name, "w");
        if (!bucket[i]) return 1;
    }

    uint32_t x;

    while (fscanf(input, "%u", &x) == 1)
    {
        if (x <= 120)
            fprintf(bucket[x], "%u\n", x);
    }

    fclose(input);

    for (int i = 0; i <= 120; i++)
        fclose(bucket[i]);

    FILE *out = fopen("sorted.txt", "w");
    if (!out) return 1;

    for (int i = 0; i <= 120; i++)
    {
        char name[32];
        sprintf(name, "bucket_%d.txt", i);

        FILE *in = fopen(name, "r");
        if (!in) continue;

        while (fscanf(in, "%u", &x) == 1)
            fprintf(out, "%u\n", x);

        fclose(in);
    }

    fclose(out);

    for (int i = 0; i <= 120; i++)
    {
        char name[32];
        sprintf(name, "bucket_%d.txt", i);
        remove(name);
    }

    remove("numbers.txt");
    rename("sorted.txt", "numbers.txt");

    return 0;
}