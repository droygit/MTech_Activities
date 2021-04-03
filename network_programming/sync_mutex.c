#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_THREAD  5
#define MAX_SLEEP_TIME 2
#define DEFAULT_VALUE 999

pthread_t thread_id[MAX_THREAD];
int data = DEFAULT_VALUE;
pthread_mutex_t mlock;
  
void* thread_func(void* arg)
{
    pthread_mutex_lock(&mlock);
    // Critical Section
    printf("\n\n++ Critical section started | thread id %ld ++", pthread_self());
    printf("\nBefore update data = %d", data);
    data = rand();
    printf("\nAfter update) data = %d", data);
    sleep(MAX_SLEEP_TIME);
    printf("\n++ Critical section finished | thread id %ld ++", pthread_self());
  
    pthread_mutex_unlock(&mlock);
  
    return NULL;
}
  
int main(void)
{
    int i = 0;
    int error;

    srand(time(0));
  
    if (pthread_mutex_init(&mlock, NULL) != 0) {
        printf("\n pthread_mutex_init has failed\n");
        return 1;
    }
  
    for (i=0; i < MAX_THREAD; ++i) {
        error = pthread_create(&(thread_id[i]),
                               NULL,
                               &thread_func, NULL);
        if (error != 0)
            printf("\nThread can't be created :[%s]",
                   strerror(error));
    }
  
    for (i=0; i < MAX_THREAD; ++i)
        pthread_join(thread_id[i], NULL);
    
    pthread_mutex_destroy(&mlock);
    
    printf("\n\n ++++ Program Ends ++++ \n");
  
    return 0;
}
