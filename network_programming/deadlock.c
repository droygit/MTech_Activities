#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_SLEEP_TIME 10
#define DEFAULT_VALUE 1000
#define MAX_THREAD  2

pthread_t thread_id[MAX_THREAD];
int data1 = DEFAULT_VALUE;
int data2 = DEFAULT_VALUE;
pthread_mutex_t resource1, resource2;
  
void* thread1_func(void* arg)
{
    // Critical Section
    printf("\n\n++ Critical section started | thread id %ld ++", pthread_self());
    printf("\nStart using data1 = %d", data1);
    data1 += 100;   // data1 = 1100
    sleep(MAX_SLEEP_TIME);
    printf("\nWaiting for data2");
    data1 += data2; // data1 = 2100
    printf("\nFinal data1 = %d", data1);
    printf("\n++ Critical section finished | thread id %ld ++", pthread_self());
  
    return NULL;
}
 
void* thread2_func(void* arg)
{
    // Critical Section
    printf("\n\n++ Critical section started | thread id %ld ++", pthread_self());
    printf("\nStarted using data2 = %d", data2);
    data2 += 100;   // data1 = 1100
    sleep(MAX_SLEEP_TIME);
    printf("\nWaiting for data1");
    data2 += data1; // data1 = 2100
    printf("\nFinal data2 = %d", data2);
    printf("\n++ Critical section finished | thread id %ld ++", pthread_self());
  
    return NULL;
}
  
int main(void)
{
    int i = 0;
    int error;

    error = pthread_create(&(thread_id[0]), NULL, &thread1_func, NULL);
    if (error != 0)
        printf("\nThread 1 can't be created :[%s]",
                strerror(error));
    
    error = pthread_create(&(thread_id[1]), NULL, &thread2_func, NULL);
    if (error != 0)
        printf("\nThread 2 can't be created :[%s]",
                strerror(error));


    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);
    
    printf("\n\n ++++ Program Ends ++++ \n");
  
    return 0;
}
