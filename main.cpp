#include <iostream>
#include "omp.h"

void first_task();
void second_task();
void third_task();
void fourth_task();
void fifth_task();

int main() {
    fifth_task();
    return 0;
}

void first_task() {
    #pragma omp parallel num_threads(8)
    {
        printf("Hello world: thread %d of %d \n",
               omp_get_thread_num(), omp_get_num_threads());
    }
}

void second_task() {

    int threads = 3;
    #pragma omp parallel if(threads > 1) num_threads(threads)
    {
        printf("First section: thread %d of %d \n",
               omp_get_thread_num(), omp_get_num_threads());
    }

    threads = 1;
    #pragma omp parallel if(threads > 1) num_threads(threads)
    {
        printf("Second section: thread %d of %d \n",
               omp_get_thread_num(), omp_get_num_threads());
    }
}

void third_task() {
    int a = 100;
    int b = 200;
    printf("Before first: a = %d, b = %d \n", a, b);
    #pragma omp parallel private(a) firstprivate(b) num_threads(2)
    {
        a += omp_get_thread_num();
        b += omp_get_thread_num();
        printf("Thread %d: a = %d, b = %d \n",
               omp_get_thread_num(), a, b);
    }
    printf("After first: a = %d, b = %d \n", a, b);

    printf("Before second: a = %d, b = %d \n", a, b);
    #pragma omp parallel shared(a) private(b) num_threads(4)
    {
        a -= omp_get_thread_num();
        b -= omp_get_thread_num();
        printf("Thread %d: a = %d, b = %d \n",
               omp_get_thread_num(), a, b);
    }
    printf("After second: a = %d, b = %d \n", a, b);
}

void fourth_task () {
    int a[] = {1, 2, 1, 13, 1, 14, 1, 85, -10, 2};
    int b[] = {-5, 3, 111, 6, 2, 4, -4, 0, 0, 12};
    #pragma omp parallel num_threads(2)
    {
    #pragma omp master
        {
            int min = INT_MAX;
            for (int i = 0; i < 10; ++i) {
                if (a[i] < min)
                    min = a[i];
            }
            printf("Thread %d min: %d \n", omp_get_thread_num(), min);
        }
    #pragma omp single
        {
            int max = INT_MIN;
            for (int i = 0; i < 10; ++i) {
                if (b[i] > max)
                    max = b[i];
            }
            printf("Thread %d max: %d \n", omp_get_thread_num(), max);
        }
    }
}

void fifth_task() {
    int d[6][8];
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 8; ++j) {
            d[i][j] = (std::rand() % (10000 + 1 ));
        }
    }
#pragma omp parallel sections
    {
#pragma omp section
        {
            int elements = 0;
            int sum = 0;
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 8; j++) {
                    sum += d[i][j];
                    elements++;
                }
            }
            printf("mid is %.2f from %d thread\n", sum * 1.0 / elements, omp_get_thread_num());
        }

#pragma omp section
        {
            int min = INT_MAX;
            int max = -1;
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 8; j++) {
                    if (min > d[i][j]) {
                        min = d[i][j];
                    }
                    if (max < d[i][j]) {
                        max = d[i][j];
                    }
                }
            }
            printf("max is %d and min is %d from %d thread\n", max, min, omp_get_thread_num());
        }

#pragma omp section
        {
            int sum = 0;
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 8; j++) {
                    if (d[i][j] % 3 == 0) {
                        sum ++;
                    }
                }
            }
            printf("sum of deviders is %d from %d thread\n", sum, omp_get_thread_num());
        }
    }
}