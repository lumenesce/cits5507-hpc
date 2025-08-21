#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// #define SIZES [1, 256, 65536, 262144, 134217728]
#define FLOAT_MIN 0
#define FLOAT_MAX 10
#define SEED 5507

const int SIZES[] = {1, 256, 65536, 262144, 16777216, 67108864}; // no. of float32s, or 4B, 1KB, 256KB, 1MB, 64MB, 256MB
const int NUM_SIZES = sizeof(SIZES) / sizeof(SIZES[0]);

float float_rand( float min, float max )
{
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

float dot_product(float *a, float *b, int n) {
    float sum = 0.0;
    for (int i = 0; i < n; i++)
        sum += a[i] * b[i];
    return sum;
}

int main(int argc, char **argv) {
    printf("Using seed: %d\n", SEED);
    srand(SEED); // set the seed

    printf("Floats are random between %d and %d\n", FLOAT_MIN, FLOAT_MAX);

    for (int i = 0; i < NUM_SIZES; i++) {
        printf("\nSize of array: %d\n", SIZES[i]);

        // Initialise the two arrays
        float *a = (float *)malloc(SIZES[i] * sizeof(float));
        float *b = (float *)malloc(SIZES[i] * sizeof(float));

        for (int j = 0; j < SIZES[i]; j++) {
            a[j] = float_rand(FLOAT_MIN, FLOAT_MAX);
            b[j] = float_rand(FLOAT_MIN, FLOAT_MAX);
        }

        // Start wall time clock
        struct timespec wall_start, wall_end;
        clock_gettime(CLOCK_MONOTONIC, &wall_start);

        // Start CPU time clock
        clock_t cpu_start = clock();

        // Calculate dot product
        float result = dot_product(a, b, SIZES[i]);

        // Calculate wall time
        clock_gettime(CLOCK_MONOTONIC, &wall_end);
        double wall_elapsed = (wall_end.tv_sec - wall_start.tv_sec) +
                        (wall_end.tv_nsec - wall_start.tv_nsec) / 1e9;

        // Calculate CPU time
        clock_t cpu_end = clock();
        double cpu_elapsed = (double)(cpu_end - cpu_start) / CLOCKS_PER_SEC;

        printf("Dot product result: %.0f\n", result);
        printf("Wall time elapsed: %f seconds\n", wall_elapsed);
        printf("CPU time elapsed: %f seconds\n", cpu_elapsed);

        free(a);
        free(b);
    }
    return 0;
}