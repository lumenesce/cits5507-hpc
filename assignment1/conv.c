/* conv_simplified.c
   Usage examples:
     ./conv_test -f f.txt -g g.txt            # read inputs, print output to stdout
     ./conv_test -f f.txt -g g.txt -o out.txt # write output to out.txt
     ./conv_test -H 1000 -W 1000 -kH 3 -kW 3  # generate random inputs
     ./conv_test -H 100 -W 200 -kH 4 -kW 4 -f f.txt -g g.txt -o out.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

/* Read an array file into a flat buffer. File format:
   H W\n
   row0\n
   row1\n
   ... */
int read_array_flat(const char *filename, float **out_buf, int *out_H, int *out_W) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;
    if (fscanf(fp, "%d %d", out_H, out_W) != 2) { fclose(fp); return -2; }
    int H = *out_H, W = *out_W;
    if (H <= 0 || W <= 0) { fclose(fp); return -3; }
    size_t n = (size_t)H * W;
    float *buf = malloc(sizeof(float) * n);
    if (!buf) { fclose(fp); return -4; }
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            if (fscanf(fp, "%f", &buf[i*(size_t)W + j]) != 1) {
                free(buf); fclose(fp); return -5;
            }
        }
    }
    fclose(fp);
    *out_buf = buf;
    return 0;
}

/* Write flat buffer to file with 3 decimal places (no trailing space at line end) */
int write_array_flat(const char *filename, const float *buf, int H, int W) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return -1;
    fprintf(fp, "%d %d\n", H, W);
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            if (j) fprintf(fp, " ");
            fprintf(fp, "%.3f", buf[i*(size_t)W + j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
}

/* allocate double-pointer (rows allocated individually) and copy from flat buffer */
float **alloc_doubleptr_from_flat(const float *flat, int H, int W) {
    float **arr = malloc(sizeof(float*) * H);
    if (!arr) return NULL;
    for (int i = 0; i < H; ++i) {
        arr[i] = malloc(sizeof(float) * W);
        if (!arr[i]) {
            for (int k = 0; k < i; ++k) free(arr[k]);
            free(arr);
            return NULL;
        }
        if (flat) memcpy(arr[i], &flat[i*(size_t)W], sizeof(float)*W);
        else memset(arr[i], 0, sizeof(float)*W);
    }
    return arr;
}

void free_doubleptr(float **arr, int H) {
    if (!arr) return;
    for (int i = 0; i < H; ++i) free(arr[i]);
    free(arr);
}

/* Naive conv2d with double-pointer interface. Handles odd and even kernels via centre definition:
   centre_row = (kH - 1) / 2; centre_col = (kW - 1) / 2;
   For each output (i,j): sum over kernel indices (ki,kj):
       src_i = i + (ki - centre_row)
       src_j = j + (kj - centre_col)
   If src outside image, treat as zero.
*/
void conv2d_naive(double **f, int H, int W, double **g, int kH, int kW, double **out) {
    int centre_r = (kH - 1) / 2;
    int centre_c = (kW - 1) / 2;

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            double sum = 0.0;
            for (int ki = 0; ki < kH; ++ki) {
                int src_i = i + (ki - centre_r);
                if (src_i < 0 || src_i >= H) continue;
                for (int kj = 0; kj < kW; ++kj) {
                    int src_j = j + (kj - centre_c);
                    if (src_j < 0 || src_j >= W) continue;
                    sum += f[src_i][src_j] * g[ki][kj];
                }
            }
            out[i][j] = sum;

        }
    }
}

/* Helper to convert float buffers (float) to double-pointer of doubles for computation precision */
double **alloc_doubleptr_from_float_flat(const float *flat, int H, int W) {
    double **arr = malloc(sizeof(double*) * H);
    if (!arr) return NULL;
    for (int i = 0; i < H; ++i) {
        arr[i] = malloc(sizeof(double) * W);
        if (!arr[i]) { for (int k=0;k<i;++k) free(arr[k]); free(arr); return NULL; }
        for (int j = 0; j < W; ++j) arr[i][j] = flat[i*(size_t)W + j];
    }
    return arr;
}

void free_doubleptr_double(double **arr, int H) { if (!arr) return; for (int i=0;i<H;++i) free(arr[i]); free(arr); }

/* Main program: parse args, read or generate, run conv, print/write output */
int main(int argc, char **argv) {
    char *file_f = NULL, *file_g = NULL, *file_o = NULL;
    int H=0,W=0,kH=0,kW=0;
    int print_stdout = 1;

    struct option long_options[] = {
        {"kH", required_argument, 0, 0},
        {"kW", required_argument, 0, 0},
        {0, 0, 0, 0} // terminator
    };

    int option_index = 0;
    int c;

    while ((c = getopt_long(argc, argv, "f:g:o:H:W:hp", long_options, &option_index)) != -1) {
        switch (c) {
            case 0: // long option
                if (strcmp(long_options[option_index].name, "kH") == 0) kH = atoi(optarg);
                if (strcmp(long_options[option_index].name, "kW") == 0) kW = atoi(optarg);
                break;
            case 'f': file_f = strdup(optarg); break;
            case 'g': file_g = strdup(optarg); break;
            case 'o': file_o = strdup(optarg); break;
            case 'H': H = atoi(optarg); break;
            case 'W': W = atoi(optarg); break;
            case 'p': print_stdout = 1; break;
            case 'h':
            default:
                fprintf(stderr, "Usage: %s ...\n", argv[0]);
                return 1;
        }
    }

    float *f_flat = NULL, *g_flat = NULL;
    int fH=0,fW=0,gH=0,gW=0;

    int generate_random = (H>0 && W>0 && kH>0 && kW>0);

    /* Only read f/g files if we're NOT generating random arrays,
    or if the file already exists (optional). */
    if (!generate_random && file_f) {
        if (read_array_flat(file_f, &f_flat, &fH, &fW) != 0) { 
            fprintf(stderr, "Failed to read f file\n"); 
            return 1; 
        }
    }
    if (!generate_random && file_g) {
        if (read_array_flat(file_g, &g_flat, &gH, &gW) != 0) { 
            fprintf(stderr, "Failed to read g file\n"); 
            return 1; 
        }
    }

    if (!f_flat && !(H>0 && W>0)) { fprintf(stderr, "Either provide -f or -H and -W\n"); return 1; }
    if (!g_flat && !(kH>0 && kW>0)) { fprintf(stderr, "Either provide -g or -kH and -kW\n"); return 1; }

    if (!f_flat) {
        fH = H; fW = W;
        printf("H = %d\n", H);
        printf("W = %d\n", W);
        f_flat = malloc(sizeof(float)*(size_t)H*W);
        if (!f_flat) { perror("malloc"); return 1; }
        srand(1234);
        for (size_t i=0;i<(size_t)H*W;++i) f_flat[i] = (float)rand()/RAND_MAX;

        /* save to file if requested */
        if (file_f) {
            if (write_array_flat(file_f, f_flat, fH, fW) != 0) {
                fprintf(stderr, "Failed to write generated f file\n");
                return 1;
            }
        }
    }

    if (!g_flat) {
        gH = kH; gW = kW;
        g_flat = malloc(sizeof(float)*(size_t)kH*kW);
        if (!g_flat) { perror("malloc"); return 1; }
        srand(5678);
        for (size_t i=0;i<(size_t)kH*kW;++i) g_flat[i] = (float)rand()/RAND_MAX;

        /* save to file if requested */
        if (file_g) {
            if (write_array_flat(file_g, g_flat, gH, gW) != 0) {
                fprintf(stderr, "Failed to write generated g file\n");
                return 1;
            }
        }
    }

    /* final dims */
    if (f_flat) { /* already set fH,fW */ }
    if (g_flat) { /* already set gH,gW */ }

    /* Kernel must not be bigger than image */
    if (gH > fH || gW > fW) { fprintf(stderr, "Kernel must not be larger than image (got f %dx%d, g %dx%d)\n", fH, fW, gH, gW); return 1; }

    /* convert to double-pointer double for conv computations */
    double **f_dp = alloc_doubleptr_from_float_flat(f_flat, fH, fW);
    double **g_dp = alloc_doubleptr_from_float_flat(g_flat, gH, gW);
    if (!f_dp || !g_dp) { fprintf(stderr, "Memory allocation failed\n"); return 1; }

    double **out_dp = malloc(sizeof(double*) * fH);
    for (int i = 0; i < fH; ++i) { out_dp[i] = malloc(sizeof(double) * fW); for (int j=0;j<fW;++j) out_dp[i][j] = 0.0; }

    /* perform convolution (timed) */
    clock_t t0 = clock();
    conv2d_naive(f_dp, fH, fW, g_dp, gH, gW, out_dp);
    clock_t t1 = clock();
    double elapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;

    /* convert output to float flat for writing */
    float *out_flat = malloc(sizeof(float)*(size_t)fH*fW);
    for (int i = 0; i < fH; ++i) for (int j = 0; j < fW; ++j) out_flat[i*(size_t)fW + j] = (float)out_dp[i][j];

    if (file_o) {
        if (write_array_flat(file_o, out_flat, fH, fW) != 0) fprintf(stderr, "Failed to write output\n");
    }

    if (print_stdout) {
        printf("%d %d\n", fH, fW);
        for (int i = 0; i < fH; ++i) {
            for (int j = 0; j < fW; ++j) {
                if (j) printf(" ");
                printf("%.3f", out_flat[i*(size_t)fW + j]);
            }
            printf("\n");
        }
    }

    fprintf(stderr, "Time: %.6f s\n", elapsed);

    /* cleanup */
    free(f_flat); free(g_flat); free(out_flat);
    free_doubleptr_double(f_dp, fH); free_doubleptr_double(g_dp, gH);
    for (int i=0;i<fH;++i) free(out_dp[i]); free(out_dp);
    if (file_f) free(file_f); if (file_g) free(file_g); if (file_o) free(file_o);
    return 0;
}
