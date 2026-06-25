#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define CHUNK_SIZE (64 * 1024 * 1024)
#define OUT_BUF_SIZE (64 * 1024)

int main()
{
    FILE *in = fopen("numbers.txt", "r");
    if (!in) return 1;

    uint32_t counts[121] = {0};

    char *buffer = malloc(CHUNK_SIZE + 1);
    if (!buffer) return 1;

    char leftover[64];
    leftover[0] = '\0';

    size_t bytes;

    while ((bytes = fread(buffer, 1, CHUNK_SIZE, in)) > 0)
    {
        buffer[bytes] = '\0';

        char temp[CHUNK_SIZE + 64];
        strcpy(temp, leftover);
        strcat(temp, buffer);

        size_t len = strlen(temp);
        size_t start = 0;

        for (size_t i = 0; i < len; i++)
        {
            if (temp[i] == '\n')
            {
                temp[i] = '\0';

                int x = atoi(temp + start);

                if (x >= 0 && x <= 120)
                    counts[x]++;

                start = i + 1;
            }
        }

        strcpy(leftover, temp + start);
    }

    if (leftover[0] != '\0')
    {
        int x = atoi(leftover);
        if (x >= 0 && x <= 120)
            counts[x]++;
    }

    fclose(in);
    free(buffer);

    FILE *out = fopen("sorted.txt", "w");
    if (!out) return 1;

    char outbuf[OUT_BUF_SIZE];
    size_t pos = 0;

    for (int i = 0; i <= 120; i++)
    {
        for (uint32_t j = 0; j < counts[i]; j++)
        {
            int len = sprintf(outbuf + pos, "%d\n", i);
            pos += len;

            if (pos > OUT_BUF_SIZE - 32)
            {
                fwrite(outbuf, 1, pos, out);
                pos = 0;
            }
        }
    }

    if (pos > 0)
        fwrite(outbuf, 1, pos, out);

    fclose(out);

    return 0;
}