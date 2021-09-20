#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define n 4096
double A[n][n];
double B[n][n];
double C[n][n];

float tdiff(struct timeval *start, struct timeval *end) {
    return (end->tv_sec - start->tv_sec) + 1e-6 * (end->tv_usec - start->tv_usec);
}

void mult() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void mult_chg_loop() {
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            for (int j = 0; j < n; ++j) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void mult_parallel() {
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            for (int j = 0; j < n; ++j) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    } 
}

void mult_tiled() {
    int s = 64;
    #pragma omp parallel for
    for (int ih = 0; ih < n; ih += s) {
        for (int jh = 0; jh < n; jh += s) {
            for (int kh = 0; kh < n; kh += s) {
                for (int il = 0; il < s; ++il) {
                    for (int kl = 0; kl < s; ++kl) {
                        for (int jl = 0; jl < s; ++jl) {
                            C[ih + il][jh + jl] += A[ih + il][kh + kl] * B[kh + kl][jh + jl];
                        }
                    }
                }
            }
        }
    }
}

void mult_tiled_two_level() {
    int s = 256;
    int t = 64;
    #pragma omp parallel for
    for (int ih = 0; ih < n; ih += s) {
        for (int jh = 0; jh < n; jh += s) {
            for (int kh = 0; kh < n; kh += s) {
                for (int im = 0; im < s; im += t) {
                    for (int jm = 0; jm < s; jm += t) {
                        for (int km = 0; km < s; km += t) {
                            for (int il = 0; il < t; ++il) {
                                for (int kl = 0; kl < t; ++kl) {
                                    for (int jl = 0; jl < t; ++jl) {
                                        C[ih + im + il][jh + jm + jl] += A[ih + im + il][kh + km + kl] 
                                            * B[kh + km + kl][jh + jm + jl];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[])  {
    int test_perf = 1;

    if (argc > 1) {
        test_perf = atoi(argv[1]);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i][j] = (double)rand() / (double)RAND_MAX;
            B[i][j] = (double)rand() / (double)RAND_MAX;
            C[i][j] = 0.0;
        }
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    switch (test_perf)
    {
        case 1:
            mult();
            break;
        case 2:
            mult_chg_loop();
            break;
        case 3:
            mult_parallel();
            break;
        case 4:
            mult_tiled();
            break;
        case 5:
            mult_tiled_two_level();
            break;
        default:
            printf("Test perf %d not implemented", test_perf);
            break;
    }        

    gettimeofday(&end, NULL);
    printf("%0.6f", tdiff(&start, &end));
    return 0;
}
