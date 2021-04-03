#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_THREAD  10
#define MAX_SLEEP_TIME 2
pthread_t thread_id[MAX_THREAD];
int data;
pthread_mutex_t mlock;
  
void* thread_func(void* arg)
{
    pthread_mutex_lock(&mlock);
  
    unsigned long i = 0;
    data = rand();
    printf("\n\nThread id %ld has started  | data = %d", pthread_self(), data);

    sleep(MAX_SLEEP_TIME);

    printf("\nThread id %ld has finished | data = %d", pthread_self(), data);
  
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
    
    printf("\n ++++ Program Ends ++++ \n");
  
    return 0;
}
