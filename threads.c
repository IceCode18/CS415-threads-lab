#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>


void *producer_job(void *arg);
void *consumer_job(void *arg);
void addtoA();
void addtoB();
int randomNumber();

int sum_A, sum_B;
sem_t a_lock, b_lock;
int main(int argc, char **argv){
    int total_threads;
    pthread_t *threads_array;
    sum_A = 0;
    sum_B = 0;

    // Check number of inputs
    if(argc > 1){
        printf("Error: No input needed. Do not pass in an argument.\n");
        return 0;
    }

    printf("Enter desired number of producers/consumers: "); 
    scanf("%d", &total_threads); 

    // Initialize number of threads and threads array
    total_threads *= 2;
    threads_array = (pthread_t*) calloc(total_threads,sizeof(pthread_t));

    // Initialize semaphores for Part B
    if(sem_init(&a_lock,0,1) < 0){
        perror("Error initializing sum_lock_A");
        return 0;
    }
    if(sem_init(&b_lock,0,1) < 0){
        perror("Error initializing sum_lock_B");
        return 0;
    }
    
    //Seed for random generator 
    srand(time(0)); 

    // Launch threads
    printf("Launching %d threads... ( %d producer(s) and %d consumer(s) )\n", total_threads, total_threads/2, total_threads/2);
    for(int i=0; i<total_threads; i++){
        long thread_id = (long)i;
        if( i%2 == 0){
            if(pthread_create(&threads_array[i], NULL, &producer_job,(void *)thread_id) != 0){
                perror("Error");
            }
        }else if( i%2 != 0){
            if(pthread_create(&threads_array[i], NULL, &consumer_job,(void *)thread_id) != 0){
                perror("Error");
            }
        }
        
    }

    // Prevent threads from terminating early
    for(int i=0; i<total_threads; i++){
        pthread_join(threads_array[i], NULL);
    }

    printf("A = %d\n", sum_A);
    printf("B = %d\n", sum_B);
    free(threads_array);
}

// Producer Thread Job
void *producer_job(void *arg){
    int t_id = (int)(long)arg;
    printf("Producer %d launched . . . \n", t_id);

}

// Consumer Thread Job
void *consumer_job(void *arg){
    int t_id = (int)(long)arg;
    printf("Consumer %d launched . . . \n", t_id);

}

int randomNumber(){
    return (rand() % (100 + 1 - 2)) + 2;
}

