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
        default:
            printf("Test perf %d not implemented", test_perf);
            break;
    }        

    gettimeofday(&end, NULL);
    printf("%0.6f", tdiff(&start, &end));
    return 0;
}
