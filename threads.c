#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
// #include <string.h>


void *do_math(void *arg);

int sum;
sem_t a_lock;

int main(int argc, char **argv){
    int num_threads;
    pthread_t *threads;
    sum = 0;

    if(argc<2){
        printf("Usage: %s [NUM ITERATIONS]\n", argv[0]);
    }

    num_threads = atoi(argv[1]);
    threads = (pthread_t*) calloc(num_threads,sizeof(pthread_t));

    if(sem_init(&a_lock,0,1) < 0){
        perror("Error initializing sum_lock");
        return 0;
    }

    printf("Launching %d threads\n", num_threads);
    for(int i=0; i<num_threads; i++){
        long thread_id = (long)i;
        if(pthread_create(&threads[i], NULL, &do_math,(void *)thread_id) != 0){
            perror("Error");
        }
    }

    for(int i=0; i<num_threads; i++){
        pthread_join(threads[i], NULL);
    }

    printf("Sum = %d\n", sum);

    free(threads);

}

void *do_math(void *arg){
    int t_id = (int)(long)arg;
    int tmp;
    printf("Thread %d launched . . . \n", t_id);
    for(int n=0; n<100; n++){
        sem_wait(&a_lock);
        tmp = sum;
        usleep(5);
        tmp++;
        sum = tmp;
        sem_post(&a_lock);
    }
}