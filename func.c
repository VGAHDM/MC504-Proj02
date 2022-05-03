#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <windows.h>


//shared variables
volatile int boarders = 0;
volatile int unboarders = 0;
volatile int C=6;
volatile int queue_size=0;

//mutex locks
pthread_mutex_t board_lock; //locks while the passenger boards, so they can update the variable and check for full car
pthread_mutex_t unboard_lock;

//semaphores
sem_t board_sem; //semaphore to allow C passengers to get in
sem_t unboard_sem; //semaphore to allow C passengers to get out
sem_t full_car; //semaphore to signal a car is full and make passengers wait
sem_t empty_car; //semaphore to signal for an empty car and make passengers get in


//car functions
void load(){
    printf("Car ready, please get in\n");
}

void unload(){
    printf("The ride is over, GET OUT!!!\n");
}

void run(){
    printf("The car is full, so the ride is about to start\n");
    printf("Hey driver, you can run, the fifth grade is not afraid to die\n");
    Sleep(1500);
}

//passenger functions
void board(){
    printf("Getting in uwu\n");
}

void unboard(){
    printf("Getting out now, that was easy\n *throws up*\n");
}


void* car_func(){
    while(1){
        /////
        printf("%d\n", queue_size);
        ////
        load();
        sem_post_multiple(&board_sem, C);
        sem_wait(&full_car);

        run();

        unload();
        sem_post_multiple(&unboard_sem, C);
        sem_wait(&empty_car);
        Sleep(500);
    }
    return NULL;
}


void* passenger_func(){
    printf("Im queueing yeeeeees\n");
    sem_wait(&board_sem); //passenger must wait for the car to signal
    
    ///
    pthread_mutex_lock(&board_lock);
    boarders++;
    board();
    if(boarders==C){
        sem_post(&full_car);
        boarders=0;
    }
    pthread_mutex_unlock(&board_lock);
    ///
    
    sem_wait(&unboard_sem); //passenger must wait for ride to be over
    unboard();

    //
    pthread_mutex_lock(&unboard_lock);
    unboarders++;
    if (unboarders == C){
        sem_post(&empty_car);
        unboarders=0;
    }
    pthread_mutex_unlock(&unboard_lock);
    //

    queue_size--;
    return NULL;
}



void main(){
    pthread_t car;
    pthread_t passenger;

    pthread_create(&car, NULL, car_func, NULL); //since there is only one car, it can be initialized here

    pthread_mutex_init(&board_lock, NULL);
    pthread_mutex_init(&unboard_lock, NULL);

    sem_init(&board_sem,0,0);
    sem_init(&unboard_sem,0,0);
    sem_init(&full_car,0,0);
    sem_init(&empty_car,0,0);

    srand(time(NULL));

    //////////////////////////////////////////////////////
    while(1){ //new passengers will join the queue indefinitely
        pthread_create(&passenger, NULL, passenger_func, NULL);
        queue_size++;
        Sleep(rand()%5000);
    }

    //since the roller coaster never stops, this wont be executed, but in case we create a maximum number of rides, this will be necessary
    pthread_join(car, NULL);
    pthread_mutex_destroy(&board_lock);
    pthread_mutex_destroy(&unboard_lock);
    sem_destroy(&board_sem);
    sem_destroy(&unboard_sem);
    sem_destroy(&full_car);
    sem_destroy(&empty_car);

}
//Precisa existir algo que incremente a queue_size toda vez que uma thread de passageiro eh criada