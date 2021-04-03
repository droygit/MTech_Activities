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
pthread_mutex_t lock1, lock2;
  
void* thread1_func(void* arg)
{
    pthread_mutex_lock(&lock1);
    printf("thread id %ld | Aquired lock1\n", pthread_self());
    sleep(MAX_SLEEP_TIME);
    printf("thread id %ld | Requesting for lock2\n", pthread_self());
    pthread_mutex_lock(&lock2);
    printf("thread id %ld | Aquired lock2\n", pthread_self());
    pthread_mutex_unlock(&lock2);

    printf("thread id %ld | Job finished!\n", pthread_self());
    pthread_mutex_unlock(&lock1);

    pthread_exit(NULL);
    return NULL;
}

void* thread2_func(void* arg)
{
    pthread_mutex_lock(&lock2);
    printf("thread id %ld | Aquired lock2\n", pthread_self());
    sleep(MAX_SLEEP_TIME);
    printf("thread id %ld | Requesting for lock1\n", pthread_self());
    pthread_mutex_lock(&lock1);
    printf("thread id %ld | Aquired lock1\n", pthread_self());
    pthread_mutex_unlock(&lock1);

    printf("thread id %ld | Job finished!\n", pthread_self());
    pthread_mutex_unlock(&lock2);

    pthread_exit(NULL);
    return NULL;
}
  
int main(void)
{
    int i = 0;
    int error;
    
    if (pthread_mutex_init(&lock1, NULL) != 0) {
        printf("\n pthread_mutex_init for mlock1 has failed\n");
        return 1;
    }
    if (pthread_mutex_init(&lock2, NULL) != 0) {
        printf("\n pthread_mutex_init for mlock2 has failed\n");
        return 1;
    }

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
    
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    
    printf("\n\n ++++ Program Ends ++++ \n");
  
    return 0;
}
