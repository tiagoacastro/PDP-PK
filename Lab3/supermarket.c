#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>     

#define VISITS 2        /* Number of visits by each customer */

int c = 5;
int b = 2;
int max_time = 10;
pthread_mutex_t* baskets;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int* buffer;

 
void* thread(void* number) {
        printf("\tCustomer #%d starts\n", (int)number);
        for (int i = 0; i < VISITS; i++)  {
                for(int j = 0;;j++){
                        if(j == b)
                                j = 0;
                        int basket = -1;
                        pthread_mutex_lock(&mutex);
                                if(buffer[j] != -1){
                                        basket = j;
                                        buffer[j] = -1;
                                }
                        pthread_mutex_unlock(&mutex);
                        if(basket != -1){
                                pthread_mutex_lock(&baskets[basket]);
                                        printf("\tCustomer #%d takes the basket #%d\n", (int)number, (basket+1));
                                        int r = rand() % max_time + 1;                                                //Random numbers from 1 to max_time
                                        sleep(r);
                                        printf("\tCustomer #%d drops the basket #%d after using it for %d seconds\n", (int)number, (basket+1), r);
                                pthread_mutex_unlock(&baskets[basket]);
                                pthread_mutex_lock(&mutex);
                                        buffer[j] = 1;
                                pthread_mutex_unlock(&mutex);
                                sleep(1);
                                break;
                        }
                }
        }
        printf("\tCustomer #%d stops\n", (int)number);
        return NULL;
}
 
int main(int argc, char **argv) {
        //Input check
	if(argc != 4 && argc != 1)
                return(-1);
        else if(argc == 4){
                sscanf (argv[1],"%d",&c);
                sscanf (argv[2],"%d",&b);
                sscanf (argv[3],"%d",&max_time);
        }


        pthread_t id[c];
        int i;
        
        baskets = (pthread_mutex_t*) malloc(b * (sizeof(pthread_mutex_t)));
        buffer = (int*) malloc(b * (sizeof(int)));

        srand(time(NULL));

        //mutex setup
        for (i=0; i < b; i++)
                pthread_mutex_init(&baskets[i], NULL);
        
        //buffer setup
        for (i=0; i < b; i++)
                buffer[i] = 1;
 
        puts("Supermarket created");
        
        //thread setup
        for (i=0; i < c; i++) {
                errno = pthread_create(&id[i], NULL, thread, (void*)(i+1));
                if (errno) {
                        perror("Error while creating the thread");
                        return EXIT_FAILURE;
                }
        }
        
        //thread joins
        for (i=0; i < c; i++) {
                if( pthread_join (id[i], NULL ) ) {
                        printf("Error while joining the thread");
                        abort();
                }
        }

        free(baskets);
        free(buffer);

        puts("Supermarket closed");
        return EXIT_SUCCESS;
}

