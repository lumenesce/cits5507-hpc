#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define SIZE 134217728 // 0.5 GB of float32s

// int default_calculation() {
//     printf("-------- DEFAULT CALCULATION ------------------\n");

//     // generate random array
//     float *array = malloc(SIZE * sizeof(float));

//     for (int i = 0; i < SIZE; i++) {
//         array[i] = rand() / (float)(RAND_MAX + 1.0);
//     }

//     double sum = 0.0;
//     double start = omp_get_wtime();

//     #pragma omp parallel
//     {
//         float local_sum = 0;
//         #pragma omp for
//         for (int i = 0; i < SIZE; i++) {
//             local_sum += array[i];
//         }
//         sum += local_sum;
//     }

//     double end = omp_get_wtime();

//     printf("Total: %.4f\n", sum);
//     printf("Elapsed time: %.6f seconds\n", end - start);

//     free(array);
//     return 0;
// }

// int critical_calculation() {
//     printf("-------- CRITICAL CALCULATION ------------------\n");
//     // generate random array
//     float *array = malloc(SIZE * sizeof(float));

//     for (int i = 0; i < SIZE; i++) {
//         array[i] = rand() / (float)(RAND_MAX + 1.0);
//     }

//     double sum = 0.0;
//     double start = omp_get_wtime();

//     #pragma omp parallel shared (sum)
//     {
//         float local_sum = 0;
//         #pragma omp for
//         for (int i = 0; i < SIZE; i++) {
//             local_sum += array[i];
//         }
//         #pragma omp critical
//         {
//             sum += local_sum;
//         }
//     }

//     double end = omp_get_wtime();

//     printf("Total: %.4f\n", sum);
//     printf("Elapsed time: %.6f seconds\n", end - start);

//     free(array);
//     return 0;
// }

// int atomic_calculation() {
//     printf("-------- ATOMIC CALCULATION ------------------\n");
//     // generate random array
//     float *array = malloc(SIZE * sizeof(float));

//     for (int i = 0; i < SIZE; i++) {
//         array[i] = rand() / (float)(RAND_MAX + 1.0);
//     }

//     double sum = 0.0;
//     double start = omp_get_wtime();

//     #pragma omp parallel shared (sum)
//     {
//         float local_sum = 0;
//         #pragma omp for
//         for (int i = 0; i < SIZE; i++) {
//             local_sum += array[i];
//         }
//         #pragma omp atomic
//             sum += local_sum;
//     }

//     double end = omp_get_wtime();

//     printf("Total: %.4f\n", sum);
//     printf("Elapsed time: %.6f seconds\n", end - start);

//     free(array);
//     return 0;
// }

int default_calculation() {
    printf("-------- DEFAULT CALCULATION, DATA RACE ------------------\n");

    // generate random array
    float *array = malloc(SIZE * sizeof(float));

    for (int i = 0; i < SIZE; i++) {
        array[i] = rand() / (float)(RAND_MAX + 1.0);
    }

    double sum = 0.0;
    double start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        sum += array[i];
    }

    double end = omp_get_wtime();

    printf("Total: %.4f\n", sum);
    printf("Elapsed time: %.6f seconds\n", end - start);

    free(array);
    return 0;
}

int unparallel_calculation() {
    printf("-------- UNPARALLELISED CALCULATION ------------------\n");

    // generate random array
    float *array = malloc(SIZE * sizeof(float));

    for (int i = 0; i < SIZE; i++) {
        array[i] = rand() / (float)(RAND_MAX + 1.0);
    }

    double sum = 0.0;
    double start = omp_get_wtime();

    for (int i = 0; i < SIZE; i++) {
        sum += array[i];
    }

    double end = omp_get_wtime();

    printf("Total: %.4f\n", sum);
    printf("Elapsed time: %.6f seconds\n", end - start);

    free(array);
    return 0;
}

int critical_calculation() {
    printf("-------- CRITICAL CALCULATION ------------------\n");
    // generate random array
    float *array = malloc(SIZE * sizeof(float));

    for (int i = 0; i < SIZE; i++) {
        array[i] = rand() / (float)(RAND_MAX + 1.0);
    }

    double sum = 0.0;
    double start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        #pragma omp critical
        sum += array[i];
    }

    double end = omp_get_wtime();

    printf("Total: %.4f\n", sum);
    printf("Elapsed time: %.6f seconds\n", end - start);

    free(array);
    return 0;
}

int atomic_calculation() {
    printf("-------- ATOMIC CALCULATION ------------------\n");
    // generate random array
    float *array = malloc(SIZE * sizeof(float));

    for (int i = 0; i < SIZE; i++) {
        array[i] = rand() / (float)(RAND_MAX + 1.0);
    }

    double sum = 0.0;
    double start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        #pragma omp atomic
        sum += array[i];
    }

    double end = omp_get_wtime();

    printf("Total: %.4f\n", sum);
    printf("Elapsed time: %.6f seconds\n", end - start);

    free(array);
    return 0;
}

int reduction_calculation() {
    printf("-------- REDUCTION CALCULATION ------------------\n");
    // generate random array
    float *array = malloc(SIZE * sizeof(float));

    for (int i = 0; i < SIZE; i++) {
        array[i] = rand() / (float)(RAND_MAX + 1.0);
    }

    double sum = 0.0;
    double start = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < SIZE; i++) {
        sum += array[i];
    }

    double end = omp_get_wtime();

    printf("Total: %.4f\n", sum);
    printf("Elapsed time: %.6f seconds\n", end - start);

    free(array);
    return 0;
}

int main(int argc, char **argv) {
    default_calculation();
    unparallel_calculation();
    critical_calculation();
    atomic_calculation();
    reduction_calculation();

    return 0;
}