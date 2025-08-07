#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SIZE 65536
#define FLOAT_MIN 0
#define FLOAT_MAX 255

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
    float *a = (float *)malloc(SIZE * sizeof(float));
    float *b = (float *)malloc(SIZE * sizeof(float));

    srand(5507);

    // for (int i = 0; i < SIZE; i++) {
    //     a[i] = 1.0;
    //     b[i] = 2.0;
    // }

    for (int i = 0; i < SIZE; i++) {
        a[i] = float_rand(FLOAT_MIN, FLOAT_MAX);
        b[i] = float_rand(FLOAT_MIN, FLOAT_MAX);
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    clock_t begin = clock();

    float result = dot_product(a, b, SIZE);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / 1e9;

    clock_t stop = clock();
    double time_spent = (double)(stop - begin) / CLOCKS_PER_SEC;

    printf("Result: %.0f\n", result);
    printf("Wall time: %f seconds\n", elapsed);
    printf("CPU time: %f seconds\n", time_spent);

    free(a);
    free(b);
    return 0;
}