#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define SIZE 134217728 // 0.5 GB of float32s

int main(int argc, char **argv) {
    // WITH SHARED ARRAY AND SUM
    printf("-------- WITH DEFAULT(NONE), SHARED(ARRAY, SUM) ------------------\n");
    // generate random array
    float *array = malloc(SIZE * sizeof(float));

    for (int i = 0; i < SIZE; i++) {
        array[i] = rand() / (float)(RAND_MAX + 1.0);
    }

    double sum = 0.0;
    double start = omp_get_wtime();

    #pragma omp parallel for default(none) shared(array, sum)
    for (int i = 0; i < SIZE; i++) {
        sum += array[i];
    }

    double end = omp_get_wtime();

    printf("Total: %.4f\n", sum);
    printf("Elapsed time: %.6f seconds\n", end - start);

    // WITH SHARED ARRAY, REDUCTION FOR SUM
    printf("-------- WITH DEFAULT(NONE), SHARED(ARRAY), REDUCTION(+:SUM) ------------------\n");
    sum = 0.0;
    start = omp_get_wtime();

    #pragma omp parallel for default(none) shared(array) reduction(+:sum)
    for (int i = 0; i < SIZE; i++) {
        sum += array[i];
    }

    end = omp_get_wtime();

    printf("Total: %.4f\n", sum);
    printf("Elapsed time: %.6f seconds\n", end - start);
    free(array);

    return 0;
}