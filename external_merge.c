#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define CHUNK_SIZE (50 * 1024 * 1024)
#define MERGE_BATCH 32

int cmp_uint32(const void *a, const void *b)
{
    uint32_t x = *(uint32_t*)a;
    uint32_t y = *(uint32_t*)b;
    return (x > y) - (x < y);
}

void create_runs(const char *input_file, int *run_count)
{
    FILE *in = fopen(input_file, "r");
    if (!in) { perror("input"); exit(1); }

    uint32_t *buffer = malloc(CHUNK_SIZE);
    if (!buffer) { perror("malloc"); exit(1); }

    int runs = 0;

    while (1)
    {
        int count = 0;

        while (count < CHUNK_SIZE / sizeof(uint32_t))
        {
            uint32_t x;
            if (fscanf(in, "%u", &x) != 1) break;
            buffer[count++] = x;
        }

        if (count == 0) break;

        qsort(buffer, count, sizeof(uint32_t), cmp_uint32);

        char name[64];
        sprintf(name, "run_%d.bin", runs);

        FILE *out = fopen(name, "wb");
        if (!out) { perror("run"); exit(1); }

        fwrite(buffer, sizeof(uint32_t), count, out);
        fclose(out);
        runs++;
    }

    free(buffer);
    fclose(in);
    *run_count = runs;
}

void merge_group(int start, int end, int out_id)
{
    int count = end - start;

    FILE **files = malloc(sizeof(FILE*) * count);
    uint32_t *values = malloc(sizeof(uint32_t) * count);
    int *active = malloc(sizeof(int) * count);

    char out_name[64];
    sprintf(out_name, "merge_%d.bin", out_id);

    FILE *out = fopen(out_name, "wb");
    if (!out) { perror("merge"); exit(1); }

    for (int i = 0; i < count; i++)
    {
        char name[64];
        sprintf(name, "run_%d.bin", start + i);

        files[i] = fopen(name, "rb");
        if (!files[i]) { perror("run open"); exit(1); }

        active[i] = fread(&values[i], sizeof(uint32_t), 1, files[i]) == 1;
    }

    while (1)
    {
        int best = -1;

        for (int i = 0; i < count; i++)
        {
            if (!active[i]) continue;
            if (best == -1 || values[i] < values[best])
                best = i;
        }

        if (best == -1) break;

        fwrite(&values[best], sizeof(uint32_t), 1, out);
        active[best] = fread(&values[best], sizeof(uint32_t), 1, files[best]) == 1;
    }

    for (int i = 0; i < count; i++)
        fclose(files[i]);

    fclose(out);
    free(files);
    free(values);
    free(active);
}

void merge_all_runs(int total_runs)
{
    int current = total_runs;

    while (current > 1)
    {
        int new_runs = 0;

        for (int i = 0; i < current; i += MERGE_BATCH)
        {
            int end = i + MERGE_BATCH;
            if (end > current) end = current;

            merge_group(i, end, new_runs);
            new_runs++;
        }

        current = new_runs;
    }

    rename("merge_0.bin", "sorted.bin");
}

int main()
{
    int run_count = 0;
    create_runs("numbers.txt", &run_count);
    merge_all_runs(run_count);
    return 0;
}

