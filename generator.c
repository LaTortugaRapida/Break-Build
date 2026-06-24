#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define TARGET_SIZE 4ULL * 1024 * 1024 * 1024

int main()
{
    FILE *fp = fopen("numbers.txt", "w");
    if (!fp) {
        perror("File open failed!");
        return 1;
    }

    srand((unsigned)time(NULL));

    uint64_t written = 0;

    while (written < TARGET_SIZE)
    {
        uint32_t num = rand() % 121;

        char buf[16];
        int len = sprintf(buf, "%u\n", num);

        if (written + len > TARGET_SIZE)
            break;

        fwrite(buf, 1, len, fp);
        written += len;
    }

    fclose(fp);
    return 0;
}