#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <algorithm>
#include <unistd.h>
#include <ctime>
#include <chrono>
using namespace std;

void first_task();
void second_task();
void third_task();
void fourth_task();
void fifth_task();
void sixth_task();
void seventh_task();
int eightth_task();

int main() {
    return eightth_task();
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

void sixth_task() {

    int a[10] = { 1,5,3,4,1,3,21,321,44,-21 };
    int b[10] = { 44, 11, 2, -43, -21, 0, 22, 23, 45, 10 };


    float sum_a = 0;
    float sum_b = 0;
#pragma omp parallel for

    for (int i = 0; i < 10; i++) {
        sum_a += a[i];
        sum_b += b[i];
    }
    printf("(without reduction) Avg a[] is: %f, Avg b[] is %f\n", sum_a / 10, sum_b / 10);
    sum_a = 0;
    sum_b = 0;
#pragma omp parallel for reduction(+:sum_a)
    for (int i = 0; i < 10; i++) {
        sum_a += a[i];
    }
#pragma omp parallel for reduction(+:sum_b)
    for (int i = 0; i < 10; i++) {
        sum_b += b[i];
    }
    printf("(with reduction) Avg a[] is: %f, Avg b[] is %f\n", sum_a / 10, sum_b / 10);
}

void seventh_task() {
    int a[12];
    int b[12];
    int c[12];
#pragma omp parallel num_threads(3)
    {
        int n = omp_get_thread_num();
        int all = omp_get_num_threads();
        printf("Number of threads: %d, Thread: %d\n", all, n);
#pragma omp for schedule(static,4)
        for (int i = 0; i < 12; i++)
        {
            a[i] = rand();
            srand(a[i] + omp_get_thread_num());
            b[i] = rand();
            srand(a[i] + b[i]);
        }
#pragma omp single
        {
            printf("Array a: ");
            for (int i = 0; i < 12; i++)
            {
                printf("%d, ", a[i]);
            }
            printf("\nArray b: ");
            for (int i = 0; i < 12; i++)
            {
                printf("%d, ", b[i]);
            }
            printf("\n");
        }

    }
#pragma omp parallel num_threads(4)
    {
        int n = omp_get_thread_num();
        int all = omp_get_num_threads();
        printf("Number of threads: %d, Thread: %d\n", all, n);
#pragma omp for schedule(dynamic,3)
        for (int i = 0; i < 12; i++)
        {
            c[i] = a[i] + b[i];
        }
#pragma omp single
        {
            printf("Array c: ");
            for (int i = 0; i < 12; i++)
            {
                printf("%d, ", c[i]);
            }
        }
    }
}

int eightth_task() {
    cout << "Height\n>";
    int rowCount;
    cin >> rowCount;
    cout << "Width\n>";
    int colCount;
    cin >> colCount;
    int** a = new int* [rowCount];
    for (int i = 0; i < rowCount; ++i)
        a[i] = new int[colCount];
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            cout << ">";
            cin >> a[i][j];
        }
    }
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    int length;
    cout << "Vector length\n>";
    cin >> length;
    int* vector = new int[length];
    int* summary = new int[length];
    for (int i = 0; i < length; i++) {
        cout << ">";
        cin >> vector[i];
        summary[i] = 0;
    }
    for (int i = 0; i < length; i++) {
        cout << vector[i] << " ";
    }
    cout << "\n";

    if (colCount != length) {
        return -1;
    }

    // non-parallel
    cout << "Non-parallel\n";
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            summary[i] += a[i][j] * vector[j];
        }
    }
    auto end = std::chrono::system_clock::now();
    for (int i = 0; i < length; i++) {
        cout << summary[i] << " ";
    }
    cout << "\n";

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

#pragma warning(disable : 4996)
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";

    cout << "Parallel\n";
    int* summary1 = new int[length];
    for (int i = 0; i < length; i++) {
        summary1[i] = 0;
    }
    auto start1 = std::chrono::system_clock::now();
#pragma omp parallel for collapse(2)
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            summary1[i] += a[i][j] * vector[j];
        }
    }
    auto end1 = std::chrono::system_clock::now();
    for (int i = 0; i < length; i++) {
        cout << summary1[i] << " ";
    }
    cout << "\n";

    std::chrono::duration<double> elapsed_seconds1 = end1 - start1;
    std::time_t end_time1 = std::chrono::system_clock::to_time_t(end1);

#pragma warning(disable : 4996)
    std::cout << "finished computation at " << std::ctime(&end_time1)
              << "elapsed time: " << elapsed_seconds1.count() << "s\n";
}


void task_9()
{
    int** d = new int* [6];
    for (int i = 0; i < 6; i++)
    {
        d[i] = new int[8];
    }
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++)
        {
            d[i][j] = rand();
        }
    }
    int max = d[0][0];
    int min = d[0][0];
#pragma omp parallel num_threads(4)
#pragma omp for collapse(2)
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
#pragma omp critical
            if (d[i][j] > max)
            {
                max = d[i][j];
            }
            if (d[i][j] < min)
            {
                min = d[i][j];
            }
        }
    }
    printf("Max of array is - %d, min is - %d", max, min);
}

void task_10()
{
    int count = 0;
    int* a = new int[30];
    for (int i = 0; i < 30; i++)
    {
        a[i] = rand();
    }
#pragma omp parallel for num_threads(4)
    for (int i = 0; i < 30; i++)
    {
        if (a[i] % 9 == 0)
        {
#pragma omp atomic
            count++;
        }
    }
    printf("Number of multiples of 9 - %d", count);
}

void task_11()
{
    const int N = 100000000;
    int* arr = new int[N];
    for (int i = 0; i < N; i++)
    {
        arr[i] = rand();
    }
    int max = NULL;
#pragma omp parallel num_threads(4)
#pragma omp for
    for (int i = 0; i < N; i++)
    {
#pragma omp critical
        if ((arr[i]> max && arr[i] % 7 == 0) || (arr[i] % 7 == 0 && max == NULL))
        {
            max = arr[i];
        }

    }
    printf("Max of array is - %d", max);
}

void task_12()
{
    omp_set_num_threads(8);
    printf("Method 1\n");
#pragma omp parallel
    {
        usleep((omp_get_num_threads() - omp_get_thread_num()) * 1000);
        printf("This is thread %1d/%1d, Hello world\n", omp_get_thread_num(), omp_get_num_threads());
    }
    printf("Method 2\n");
#pragma omp parallel
    {
        auto start = chrono::system_clock::now();
        auto now = chrono::system_clock::now();
        chrono::duration<double> elapsed = now - start;
        while (elapsed.count() < omp_get_num_threads() - omp_get_thread_num())
        {
            now = chrono::system_clock::now();
            elapsed = now - start;
        }
        printf("This is thread %1d/%1d, Hello world\n", omp_get_thread_num(), omp_get_num_threads());
    }
}

void task_13()
{
    int a[30] = { 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1 };
    int sum = 0;
#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < 30; i++) {
        if (a[i] == 0)
        {
            continue;
        }
        int b = 1;
#pragma omp parallel for
        for (int j = 0; j < i; j++)
        {
            b *= 2;
        }
        sum += b;
    }
    printf("%d", sum);
}

void task_14()
{
    int a = 210;
    int sum = 0;
#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < a; i++) {
        sum += i * 2 + 1;
    }
    printf("%d", sum);
}

void task_15()
{
    int a, b;
    cout << "Left interval: >";
    cin >> a;
    cout << "Right interval: >";
    cin >> b;
#pragma omp parallel for
    for (int i = a; i <= b; i++)
    {
        bool multiple = false;
        for (int j = 2; j <= sqrt(i); j++)
        {
            if (i % j == 0)
            {
                multiple = true;
                break;
            }
        }
        if (!multiple)
        {
            printf("%d\n", i);
        }
    }
}