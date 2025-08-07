#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <immintrin.h>

#define SIZE 65536

float dot_avx512(float *a, float *b, int n) {
    __m512 vsum = _mm512_setzero_ps();

    for (int i = 0; i <= n - 16; i += 16) {
        __m512 va = _mm512_loadu_ps(&a[i]);
        __m512 vb = _mm512_loadu_ps(&b[i]);
        __m512 vmul = _mm512_mul_ps(va, vb);
        vsum = _mm512_add_ps(vsum, vmul);
    }

    float sum = _mm512_reduce_add_ps(vsum);

    return sum;
}

int main(int argc, char **argv) {
    float *a = (float *)malloc(SIZE * sizeof(float));
    float *b = (float *)malloc(SIZE * sizeof(float));

    for (int i = 0; i < SIZE; i++) {
        a[i] = 1.0;
        b[i] = 2.0;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    float result = dot_avx512(a, b, SIZE);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Result: %.0f\n", result);
    printf("Wall time: %f seconds\n", elapsed);

    free(a);
    free(b);
    return 0;
}