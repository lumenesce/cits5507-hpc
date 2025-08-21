#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <unistd.h> // for usleep()

// #define N 5

struct timespec wall_start, wall_end;
clock_t cpu_start, cpu_end;
double wall_elapsed, cpu_elapsed;

void start_clocks(void) {
    clock_gettime(CLOCK_MONOTONIC, &wall_start);
    cpu_start = clock();
}

void stop_clocks(double *wall_elapsed, double *cpu_elapsed) {
    clock_gettime(CLOCK_MONOTONIC, &wall_end);
    *wall_elapsed = (wall_end.tv_sec - wall_start.tv_sec) +
                   (wall_end.tv_nsec - wall_start.tv_nsec) / 1e9;
    cpu_end = clock();
    *cpu_elapsed = (double)(cpu_end - cpu_start) / CLOCKS_PER_SEC;
}

int main(int argc, char **argv) {
    int N;

    printf("-------- NO PARALLELISATION, 10 STEPS ------------------\n");
    N = 10;
    start_clocks();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - i; j++) {
            usleep(10000);
        }
    }
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);

    printf("-------- NO PARALLELISATION, 20 STEPS ------------------\n");
    N = 20;
    start_clocks();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - i; j++) {
            usleep(10000);
        }
    }
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);
    
    printf("-------- PARALLELISATION, 10 STEPS, 2 THREADS ------------------\n");
    omp_set_num_threads(2); // use 2 threads
    N = 10;
    start_clocks();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - i; j++) {
            usleep(10000);
        }
    }
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);

    printf("-------- PARALLELISATION, 10 STEPS, 4 THREADS ------------------\n");
    omp_set_num_threads(4); // use 4 threads
    N = 10;
    start_clocks();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - i; j++) {
            usleep(10000);
        }
    }
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);

    printf("-------- PARALLELISATION, 20 STEPS, 4 THREADS ------------------\n");
    N = 20;
    start_clocks();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - i; j++) {
            usleep(10000);
        }
    }
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);

    printf("-------- PARALLELISATION, 10 STEPS, 4 THREADS, STATIC SCHEDULING [chunk size 1] ------------------\n");
    N = 10;
    start_clocks();
    #pragma omp parallel for schedule(static, 1)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - i; j++) {
            usleep(10000);
        }
    }
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);

    printf("-------- PARALLELISATION, 10 STEPS, 4 THREADS, STATIC SCHEDULING [chunk size 2] ------------------\n");
    N = 10;
    start_clocks();
    #pragma omp parallel for schedule(static, 2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - i; j++) {
            usleep(10000);
        }
    }
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);

    printf("-------- PARALLELISATION, 10 STEPS, 4 THREADS, DYNAMIC SCHEDULING [chunk size 1] ------------------\n");
    N = 10;
    start_clocks();
    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - i; j++) {
            usleep(10000);
        }
    }
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);

    printf("-------- PARALLELISATION, 10 STEPS, 4 THREADS, DYNAMIC SCHEDULING [chunk size 2] ------------------\n");
    N = 10;
    start_clocks();
    #pragma omp parallel for schedule(dynamic, 2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - i; j++) {
            usleep(10000);
        }
    }
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);

    printf("-------- PARALLELISATION, 10 STEPS, 4 THREADS, GUIDED SCHEDULING [chunk size 1] ------------------\n");
    N = 10;
    start_clocks();
    #pragma omp parallel for schedule(guided, 1)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - i; j++) {
            usleep(10000);
        }
    }
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);

    printf("-------- PARALLELISATION, 10 STEPS, 4 THREADS, GUIDED SCHEDULING [chunk size 2] ------------------\n");
    N = 10;
    start_clocks();
    #pragma omp parallel for schedule(guided, 2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - i; j++) {
            usleep(10000);
        }
    }
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);

    printf("-------- PARALLELISATION, 10 STEPS, 4 THREADS, COLLAPSE CLAUSE ------------------\n");
    N = 10;
    start_clocks();
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - i; j++) {
            usleep(10000);
        }
    }
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);
}