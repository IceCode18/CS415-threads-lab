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
void insert(int id, int value);
int take(int id);

int sum_A, sum_B, buffer_count;
sem_t a_lock, b_lock, s_lock, n_lock, e_lock;
int *buffer;

int main(int argc, char **argv){
    int total_threads;
    pthread_t *threads_array;
    sum_A = 0;
    sum_B = 0;
    buffer_count = 0;
    const int BUFFER_SIZE = 3;

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
    buffer = (int*) calloc(BUFFER_SIZE, sizeof(int));

    // Initialize semaphores for Part B
    if(sem_init(&a_lock,0,1) < 0){
        perror("Error initializing sum_lock_A");
        return 0;
    }
    if(sem_init(&b_lock,0,1) < 0){
        perror("Error initializing sum_lock_B");
        return 0;
    }
    // Initialize semaphores for Part C
    if(sem_init(&s_lock,0,1) < 0){
        perror("Error initializing buffer_lock_S");
        return 0;
    }
    if(sem_init(&n_lock,0,0) < 0){
        perror("Error initializing buffer_lock_N");
        return 0;
    }
    if(sem_init(&e_lock,0,BUFFER_SIZE) < 0){
        perror("Error initializing buffer_lock_E");
        return 0;
    }

    //Seed for random generator 
    srand(time(0)); 

    // Launch threads
    printf("Launching a total of %d threads... ( %d producer(s) and %d consumer(s) )\n", total_threads, total_threads/2, total_threads/2);
    for(int i=1; i<=total_threads; i++){
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
    free(buffer);
}

// Producer Thread Job
void *producer_job(void *arg){
    int t_id = (int)(long)arg;
    printf("Producer %d launched . . . \n", t_id);
    for(int n=0; n<100; n++){
        addtoA();
        addtoB();
    }
    for(int m=0; m<20; m++){
        int num = randomNumber();
        printf("Producer %d generated %d.\n", t_id, num); //produce
        sem_wait(&e_lock);
        sem_wait(&s_lock);
        insert(t_id, num); //append
        sem_post(&s_lock);
        sem_post(&n_lock);
    }
}

// Consumer Thread Job
void *consumer_job(void *arg){
    int t_id = (int)(long)arg;
    printf("Consumer %d launched . . . \n", t_id);
    for(int n=0; n<100; n++){
        addtoB();
        addtoA();
    }
    for(int m=0; m<20; m++){
        sem_wait(&n_lock);
        sem_wait(&s_lock);
        int num = take(t_id); //take
        sem_post(&s_lock);
        sem_post(&e_lock);
        printf("Consumer %d consumed %d.\n", t_id, num); //consume
    }
}

int randomNumber(){
    return (rand() % (100 + 1 - 2)) + 2;
}

// A operation
void addtoA(){
    sem_wait(&a_lock);
    sum_A+=1;
    usleep(randomNumber());
    sem_post(&a_lock);
}
// B operation
void addtoB(){
    sem_wait(&b_lock);
    sum_B+=3;
    usleep(randomNumber());
    sem_post(&b_lock);
}

// Insert Operation
void insert(int id, int value){
    buffer[buffer_count] = value;
    buffer_count++;
    printf("Producer %d added %d to buffer. Buffer count: %d\n", id, value, buffer_count);
    

}
// Take Operation
int take(int id){
    int num = buffer[buffer_count-1];
    buffer_count--;
    printf("Consumer %d removed %d from buffer. Buffer count: %d\n", id, num, buffer_count);   
    return num;
}
