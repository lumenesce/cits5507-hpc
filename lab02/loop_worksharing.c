#include <stdio.h>
#include <time.h>
#include <omp.h>

int main(int argc, char **argv) {
    // omp_set_num_threads(4); // always use 4 threads
    // #pragma omp parallel
    // {
    // #pragma omp for
    // for (int i = 0; i < 4; i++) {
    //     printf("thread %d executed loop iteration %d\n", omp_get_thread_num(), i);
    // }
    // }
    // printf("\n");

    // omp_set_num_threads(4); // always use 4 threads
    // #pragma omp parallel
    // {
    // #pragma omp for
    // for (int i = 0; i < 8; i++) {
    //     printf("thread %d executed loop iteration %d\n", omp_get_thread_num(), i);
    // }
    // }
    // printf("\n");

    // omp_set_num_threads(8); // always use 8 threads
    // #pragma omp parallel
    // {
    // #pragma omp for
    // for (int i = 0; i < 4; i++) {
    //     printf("thread %d executed loop iteration %d\n", omp_get_thread_num(), i);
    // }
    // }
    // printf("\n");

    // omp_set_num_threads(8); // always use 8 threads
    // #pragma omp parallel
    // {
    // #pragma omp for
    // for (int i = 0; i < 8; i++) {
    //     printf("thread %d executed loop iteration %d\n", omp_get_thread_num(), i);
    // }
    // }


    // SCHEDULING EXERCISES
    omp_set_num_threads(4); // always use 4 threads
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < 8; i++) {
        printf("thread %d executed loop iteration %d\n", omp_get_thread_num(), i);
    }
    printf("\n");

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < 8; i++) {
        printf("thread %d executed loop iteration %d\n", omp_get_thread_num(), i);
    }
    printf("\n");

    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < 8; i++) {
        printf("thread %d executed loop iteration %d\n", omp_get_thread_num(), i);
    }
    printf("\n");
}