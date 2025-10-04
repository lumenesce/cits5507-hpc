#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void matmul(float *A, float *B, float *C, int m, int n, int p) {
    // Optional: transpose B for better cache performance
    float *B_T = malloc(sizeof(float) * n * p);
    #pragma omp parallel for
    for (int i = 0; i < n; i++)
        for (int j = 0; j < p; j++)
            B_T[j * n + i] = B[i * p + j]; // transpose B

    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                sum += A[i * n + k] * B_T[j * n + k];
            }
            C[i * p + j] = sum;
        }
    }

    free(B_T);
}

int main() {
    int m = 1024, n = 1024, p = 1024;
    float *A = malloc(sizeof(float) * m * n);
    float *B = malloc(sizeof(float) * n * p);
    float *C = malloc(sizeof(float) * m * p);

    // Initialize matrices with random floats
    for (int i = 0; i < m * n; i++) A[i] = (float)rand() / RAND_MAX;
    for (int i = 0; i < n * p; i++) B[i] = (float)rand() / RAND_MAX;

    double start = omp_get_wtime();
    matmul(A, B, C, m, n, p);
    double end = omp_get_wtime();

    double elapsed = end - start;
    double flops = 2.0 * m * n * p / elapsed;

    printf("Time: %.6f s\n", elapsed);
    printf("FLOPS: %.2e\n", flops);

    free(A);
    free(B);
    free(C);
    return 0;
}