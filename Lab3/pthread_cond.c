#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>     // sleep
 
pthread_mutex_t mutex   = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond    = PTHREAD_COND_INITIALIZER; 
char condition = 0;
 
void* thread(void* number) {
        printf("\tThread #%d starts\n", (int)number);
        while (1) {
                pthread_mutex_lock(&mutex);
                do {
                        if (condition)
                                break;
                        else {
                                printf("\tThread #%d is waiting for signal...\n", (int)number);
                                pthread_cond_wait(&cond, &mutex);                                printf("\t... Thread #%d recieved signal!\n", (int)number);
                        }
                } while (1);
                pthread_mutex_unlock(&mutex);
                /* ... */
        }
 
        return NULL;
}
 
#define N 5     /* threads number */
 
int main() {
        pthread_t id[N];
        int i;
 
        puts("Beginning of the program");
 
        for (i=0; i < N; i++) {
                errno = pthread_create(&id[i], NULL, thread, (void*)(i+1));
                if (errno) {
                        perror("pthread_create");
                        return EXIT_FAILURE;
                }
        }
 
        /* sending signals */
 
        sleep(1);
        puts("pthread_cond_signal");
        pthread_cond_signal(&cond); 
        sleep(1);
        puts("pthread_cond_broadcast");
        pthread_cond_broadcast(&cond); 
        sleep(1);
 
        /* end of main process - we don't care about threads */
        puts("End of program");
        return EXIT_SUCCESS;
}

