#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#define START_FROM 10000000
#define STOP_AT 90000000

// We need this because pthread only accepts one argument
typedef struct range
{
    int from;
    int to;
} range;

int sumOfDigits(int number)
{
    int total = 0;
    while (number > 0)
    {
        int digit = number % 10;
        number = number / 10;
        total += digit;
    }
    return total;
}

// Function to be passed to thread
void *harshadNumberFinder(void *vargp)
{
    range *myRange = (range *)vargp;

    if (myRange->from == 0)
        myRange->from = 1;

    for (int i = myRange->from; i < myRange->to; i++)
    {
        if (i % sumOfDigits(i) == 0)
        {
            int x = 5;
        }
    }

    return NULL;
}

// Spawns threads and passes a range to search for harshad numbers
void calcHarshadNumber(int threadCount, int from, int to)
{
    if (threadCount == 1)
    {
        range *r = malloc(sizeof(range));
        r->from = from;
        r->to = to;
        harshadNumberFinder((void *)r);
    }
    else
    {
        pthread_t tid[threadCount];
        int size = to - from;
        int rangeSize = size / threadCount;
        for (int i = 0; i < threadCount; i++)
        {
            range *r = malloc(sizeof(range));

            r->from = rangeSize * i + from;
            r->to = r->from + rangeSize;
            pthread_create(&tid[i], NULL, harshadNumberFinder, (void *)r);
        }

        for (int i = 0; i < threadCount; i++)
        {
            pthread_join(tid[i], NULL);
        }
    }
}

int main()
{
    // Run the algorithm with 16 threads and track execution time
    struct timeval t1, t2;

    gettimeofday(&t1, NULL);
    calcHarshadNumber(16, START_FROM, STOP_AT);
    gettimeofday(&t2, NULL);

    double multiThreadedElapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
    multiThreadedElapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
    printf("Multi threaded execution time %fms\n", multiThreadedElapsedTime);

    // Run the algorithm on the main thread and track execution time
    gettimeofday(&t1, NULL);
    calcHarshadNumber(1, START_FROM, STOP_AT);
    gettimeofday(&t2, NULL);

    double singleThreadedElapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
    singleThreadedElapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
    printf("Single threaded execution time %fms\n", singleThreadedElapsedTime);
    
    return 0;
}