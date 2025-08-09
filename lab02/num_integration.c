#include <stdio.h>
#include <time.h>
#include <omp.h>

struct timespec wall_start, wall_end;
clock_t cpu_start, cpu_end;

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

double default_pi_calculation(int num_steps) {
    double step = 1.0 / (double) num_steps;
    double sum = 0;

    for (int i = 0; i < num_steps; i++) {
        double x = (i+0.5) * step;
        sum += 4.0 / (1.0 + x*x);
    }

    double pi = step * sum;
    return pi;
}

double parallel_pi_calculation(int num_steps) {
    double step = 1.0 / (double) num_steps;
    double sum = 0;

    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        printf("%d\n", num_threads);

        #pragma omp for reduction(+:sum)
        for (int i = 0; i < num_steps; i++) {
            double x = (i+0.5) * step;
            sum += 4.0 / (1.0 + x*x);
        }
    }

    double pi = step * sum;
    return pi;
}

int main(int argc, char **argv) {
    double wall_elapsed, cpu_elapsed;

    // No parallelisation
    start_clocks();
    double default_pi = default_pi_calculation(100000);
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("-------- NO PARALLELISATION, X STEPS ------------------\n");
    printf("pi=%.6f\n", default_pi);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);

    // Parallelisation ([] threads)
    start_clocks();
    double parallel_pi_1 = parallel_pi_calculation(100000);
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("-------- PARALLELISATION, X STEPS, X THREADS ----------\n");
    printf("pi=%.10f\n", parallel_pi_1);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);

    // Parallelisation with increased number of steps
    start_clocks();
    double parallel_pi_2 = parallel_pi_calculation(10000000);
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("-------- PARALLELISATION, X STEPS, X THREADS ----------\n");
    printf("pi=%.10f\n", parallel_pi_2);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds\n\n", cpu_elapsed);

    // Parallelisation with increased number of threads ([] threads)
    omp_set_num_threads(8); // always use X threads
    start_clocks();
    double parallel_pi_3 = parallel_pi_calculation(10000000);
    stop_clocks(&wall_elapsed, &cpu_elapsed);
    printf("-------- PARALLELISATION, X STEPS, X THREADS ----------\n");
    printf("pi=%.10f\n", parallel_pi_3);
    printf("Wall time elapsed: %f seconds\n", wall_elapsed);
    printf("CPU time elapsed: %f seconds", cpu_elapsed);

    return 0;
}