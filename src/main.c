#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>

//shared variables
volatile int boarders = 0;
volatile int unboarders = 0;
volatile int C=6;
volatile int queue_size=0;

//mutex locks
pthread_mutex_t board_lock; //locks while the passenger boards, so they can update the variable and check for full car
pthread_mutex_t unboard_lock;
pthread_mutex_t queue_lock;

pthread_mutex_t board_control;
pthread_mutex_t unboard_control;

//semaphores
sem_t board_sem; //semaphore to allow C passengers to get in
sem_t unboard_sem; //semaphore to allow C passengers to get out
sem_t full_car; //semaphore to signal a car is full and make passengers wait
sem_t empty_car; //semaphore to signal for an empty car and make passengers get in

//animations
WINDOW *win;

void waddstr_trunc(WINDOW *win, const char *str){
  int cur_x, max_x, dummy;
  getyx(win, dummy, cur_x);
  getmaxyx(win, dummy, max_x);
  int w=max_x - cur_x;
  if (w <= 0) return;
  char *str2 = strndup(str, w);
  if (str2 == NULL) return;
  waddstr(win, str2);
}

void negative_print(WINDOW *win, const char *str, int row, int col){
	if(strlen(str)<= (-1*col)) return;
	char str2[strlen(str)+col];
	sprintf(str2, "%s", str+(-1*col));
	move(row,0);
	waddstr_trunc(win,str2);
}

void draw_header(WINDOW *win, int state){
	//state: 1 = Boarding, 2 = Leaving, 3 = Arriving, 4 = Unboarding
	int max_row,max_col;		
	getmaxyx(win,max_row,max_col);

	switch(state){
		case(1):
			attron(COLOR_PAIR(2));
			mvprintw(1,(max_col-14)/2,"%s","+------------+");
			mvprintw(2,(max_col-14)/2,"%s","|  Boarding  |");
			mvprintw(3,(max_col-14)/2,"%s","+------------+");
			attroff(COLOR_PAIR(2));
			break;
		case(2):
			attron(COLOR_PAIR(4));
			mvprintw(1,(max_col-14)/2,"%s","+------------+");
			mvprintw(2,(max_col-14)/2,"%s","|  Leaving   |");
			mvprintw(3,(max_col-14)/2,"%s","+------------+");
			attroff(COLOR_PAIR(4));
			break;
		case(3):
			attron(COLOR_PAIR(4));
			mvprintw(1,(max_col-14)/2,"%s","+------------+");
			mvprintw(2,(max_col-14)/2,"%s","|  Arriving  |");
			mvprintw(3,(max_col-14)/2,"%s","+------------+");
			attroff(COLOR_PAIR(4));
			break;
		default:
			attron(COLOR_PAIR(3));
			mvprintw(1,(max_col-14)/2,"%s","+------------+");
			mvprintw(2,(max_col-14)/2,"%s","| Unboarding |");
			mvprintw(3,(max_col-14)/2,"%s","+------------+");
			attroff(COLOR_PAIR(3));
			break;
	}
	attron(COLOR_PAIR(1));
}

void draw_queue(WINDOW *win){
	int max_row,max_col;		
	getmaxyx(win,max_row,max_col);

	for(int i = 1; i <= queue_size-1; i++){
			if((max_col/2 - (3*i+1)) <= 0) break;
			move(12,(max_col/2) - (3*i+1));
			waddstr_trunc(win," o ");
			move(13,(max_col/2) - (3*i+1));
			waddstr_trunc(win,"/|\\");
			move(14,(max_col/2) - (3*i+1));
			waddstr_trunc(win,"/ \\");
	}
	move(11,0);
	attron(A_UNDERLINE);
	waddstr_trunc(win,"Entrada");;
	move(11,max_col-5);
	waddstr_trunc(win,"Saida");
	attroff(A_UNDERLINE);

	move(11,(max_col)/2);
	waddstr_trunc(win,"|");
	move(12,max_col/2);
	waddstr_trunc(win,"|");
	move(13,max_col/2);
	waddstr_trunc(win,"|");
	move(14,max_col/2);
	waddstr_trunc(win,"|");
	move(15,0);
	waddstr_trunc(win,"__________________________________________________________________________________________________________________________________________________________________");
	move(15,(max_col)/2);
	waddstr_trunc(win,"|");	
}

void arrive(WINDOW *win){
	int max_row,max_col, car_len = strlen(" | |___| |___| |-| |___| |___| |-| |___| |___|   )");		
	getmaxyx(win,max_row,max_col);

	for(int i = (-1*car_len); i <= 0; i++){
		clear();
		draw_header(win,3);

		negative_print(win, " __ \\o/ _ \\o/ _   _ \\o/ _ \\o/ _   _ \\o/ _ \\o/ __", 5, i);
		negative_print(win," | |_|_| |_|_| | | |_|_| |_|_| | | |_|_| |_|_|  \\ ", 6, i);
		negative_print(win," | |___| |___| |-| |___| |___| |-| |___| |___|   )", 7, i);
		negative_print(win," |_____________| |_____________| |______________/", 8, i);
		move(9,0);
		waddstr_trunc(win,"----------------------------------------------------------------------------------------------------------------------------------------------------------");
		move(10,0);
		waddstr_trunc(win,"__________________________________________________________________________________________________________________________________________________________________");

		draw_queue(win);

		refresh();

		if(50000 + (i*1000)>20000)
		usleep(60000+(i*1000));
		else usleep(20000);
	}
}


void run(WINDOW *win){
	int max_row,max_col;
	getmaxyx(win,max_row,max_col);
	
	for(int i = 0; i<max_col; i++){
		clear();
		draw_header(win,2);
	
		move(5,i);
		waddstr_trunc(win," __ \\o/ _ \\o/ _   _ \\o/ _ \\o/ _   _ \\o/ _ \\o/ __");
		move(6,i);
		waddstr_trunc(win," | |_|_| |_|_| | | |_|_| |_|_| | | |_|_| |_|_|  \\ ");
		move(7,i);
		waddstr_trunc(win," | |___| |___| |-| |___| |___| |-| |___| |___|   )");
		move(8,i);
		waddstr_trunc(win," |_____________| |_____________| |______________/");
		move(9,0);
		waddstr_trunc(win,"----------------------------------------------------------------------------------------------------------------------------------------------------------");
		move(10,0);
		waddstr_trunc(win,"__________________________________________________________________________________________________________________________________________________________________");

 		draw_queue(win);
		
		refresh();
		if(!i) usleep(1000000);
		if(50000-(i*1000)>12000)
		usleep(60000-(i*1000));
		else usleep(12000);
	}

	usleep(300000);
	arrive(win);
}

void board(WINDOW *win, int queue, int boarders){
    pthread_mutex_lock(&queue_lock);
    queue_size--;
    pthread_mutex_unlock(&queue_lock);

	int max_row,max_col;		
	getmaxyx(win,max_row,max_col);
	
	clear();
	draw_header(win, 1);
	
	move(5,0);
	switch(boarders){
		case(0):
			waddstr_trunc(win," __     _     _   _     _     _   _     _     __");
			move(6,0);
			waddstr_trunc(win," | |___| |___| | | |___| |___| | | |___| |___|  \\");
			break;
		case(1):
			waddstr_trunc(win," __     _     _   _     _     _   _     _ \\o/ __");
			move(6,0);
			waddstr_trunc(win," | |___| |___| | | |___| |___| | | |___| |_|_|  \\");
			break;
		case(2):
			waddstr_trunc(win," __     _     _   _     _     _   _ \\o/ _ \\o/ __");
			move(6,0);
			waddstr_trunc(win," | |___| |___| | | |___| |___| | | |_|_| |_|_|  \\");
			break;
		case(3):
			waddstr_trunc(win," __     _     _   _     _ \\o/ _   _ \\o/ _ \\o/ __");
			move(6,0);
			waddstr_trunc(win," | |___| |___| | | |___| |_|_| | | |_|_| |_|_|  \\");
			break;
		case(4):
			waddstr_trunc(win," __     _     _   _ \\o/ _ \\o/ _   _ \\o/ _ \\o/ __");
			move(6,0);
			waddstr_trunc(win," | |___| |___| | | |_|_| |_|_| | | |_|_| |_|_|  \\");
			break;
		case(5):
			waddstr_trunc(win," __     _ \\o/ _   _ \\o/ _ \\o/ _   _ \\o/ _ \\o/ __");
			move(6,0);
			waddstr_trunc(win," | |___| |_|_| | | |_|_| |_|_| | | |_|_| |_|_|  \\");
			break;
		default:
			waddstr_trunc(win," __ \\o/ _ \\o/ _   _ \\o/ _ \\o/ _   _ \\o/ _ \\o/ __");
			move(6,0);
			waddstr_trunc(win," | |_|_| |_|_| | | |_|_| |_|_| | | |_|_| |_|_|  \\");
			break;
	}	
	
	move(7,0);
	waddstr_trunc(win," | |___| |___| |-| |___| |___| |-| |___| |___|   )");
	move(8,0);
	waddstr_trunc(win," |_____________| |_____________| |______________/");
	move(9,0);
	waddstr_trunc(win,"----------------------------------------------------------------------------------------------------------------------------------------------------------");
	move(10,0);
	waddstr_trunc(win,"__________________________________________________________________________________________________________________________________________________________________");
	
	move(10,((max_col)/2)-5);
	waddstr_trunc(win,"|  |");
	

	draw_queue(win);

	refresh();
	usleep(500000);
	
}

void unboard(WINDOW *win, int unboarders){
	int max_row,max_col;		
	getmaxyx(win,max_row,max_col);
	
	for(int i = max_col/2; i<max_col-5;i++){

		clear();
		draw_header(win, 4); 

		move(5,0);
		switch(unboarders){
			case(5):
				waddstr_trunc(win," __     _     _   _     _     _   _     _     __");
				move(6,0);
				waddstr_trunc(win," | |___| |___| | | |___| |___| | | |___| |___|  \\");
				break;
			case(4):
				waddstr_trunc(win," __     _     _   _     _     _   _     _ \\o/ __");
				move(6,0);
				waddstr_trunc(win," | |___| |___| | | |___| |___| | | |___| |_|_|  \\");
				break;
			case(3):
				waddstr_trunc(win," __     _     _   _     _     _   _ \\o/ _ \\o/ __");
				move(6,0);
				waddstr_trunc(win," | |___| |___| | | |___| |___| | | |_|_| |_|_|  \\");
				break;
			case(2):
				waddstr_trunc(win," __     _     _   _     _ \\o/ _   _ \\o/ _ \\o/ __");
				move(6,0);
				waddstr_trunc(win," | |___| |___| | | |___| |_|_| | | |_|_| |_|_|  \\");
				break;
			case(1):
				waddstr_trunc(win," __     _     _   _ \\o/ _ \\o/ _   _ \\o/ _ \\o/ __");
				move(6,0);
				waddstr_trunc(win," | |___| |___| | | |_|_| |_|_| | | |_|_| |_|_|  \\");
				break;
			case(0):
				waddstr_trunc(win," __     _ \\o/ _   _ \\o/ _ \\o/ _   _ \\o/ _ \\o/ __");
				move(6,0);
				waddstr_trunc(win," | |___| |_|_| | | |_|_| |_|_| | | |_|_| |_|_|  \\");
				break;
			default:
				waddstr_trunc(win," __ \\o/ _ \\o/ _   _ \\o/ _ \\o/ _   _ \\o/ _ \\o/ __");
				move(6,0);
				waddstr_trunc(win," | |_|_| |_|_| | | |_|_| |_|_| | | |_|_| |_|_|  \\");
				break;
		}	
		
		move(7,0);
		waddstr_trunc(win," | |___| |___| |-| |___| |___| |-| |___| |___|   )");
		move(8,0);
		waddstr_trunc(win," |_____________| |_____________| |______________/");
		move(9,0);
		waddstr_trunc(win,"----------------------------------------------------------------------------------------------------------------------------------------------------------");
		move(10,0);
		waddstr_trunc(win,"__________________________________________________________________________________________________________________________________________________________________");
		move(10,((max_col)/2)+5);
		waddstr_trunc(win,"|  |");

		draw_queue(win);

		move(12,i+5);
		waddstr_trunc(win," o ");
		move(13,i+5);
		waddstr_trunc(win,"/|\\");
		move(14,i+5);
		waddstr_trunc(win,"/ \\");

		refresh();
		usleep(20000);
	}	
}

//threads
void* car_func(){
    while(1){
        /////
        //load();
        for (int i = 0; i < C; i++)
        {
            sem_post(&board_sem); //signals C passengers to get in
        }
        sem_wait(&full_car);

        run(win);

        //unload();
        for (int i = 0; i < C; i++)
        {
            sem_post(&unboard_sem); //signals C passengers to get out
        }
        sem_wait(&empty_car);
        usleep(500000);
    }
    return NULL;
}


void* passenger_func(){
    //printf("Im queueing yeeeeees\n");
    sem_wait(&board_sem); //passenger must wait for the car to signal
    
    ///
    pthread_mutex_lock(&board_lock);
    board(win,queue_size,boarders);
    boarders++;
    if(boarders==C){
        sem_post(&full_car);
        boarders=0;
    }
    pthread_mutex_unlock(&board_lock);
    ///

    sem_wait(&unboard_sem); //passenger must wait for ride to be over
    //
    pthread_mutex_lock(&unboard_lock);
    unboard(win,unboarders);
    unboarders++;
    if (unboarders == C){
        sem_post(&empty_car);
        unboarders=0;
    }
    pthread_mutex_unlock(&unboard_lock);
    //

    return NULL;
}


void main(){
    pthread_t car;
    pthread_t passenger;

    pthread_create(&car, NULL, car_func, NULL); //since there is only one car, it can be initialized here

    pthread_mutex_init(&board_lock, NULL);
    pthread_mutex_init(&unboard_lock, NULL);
    pthread_mutex_init(&queue_lock, NULL);

    sem_init(&board_sem,0,0);
    sem_init(&unboard_sem,0,0);
    sem_init(&full_car,0,0);
    sem_init(&empty_car,0,0);

    srand(time(NULL));

    int max_row,max_col;	
	initscr();	
    win = stdscr;		
	getmaxyx(stdscr,max_row,max_col);	
	
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	clear();
	attron(COLOR_PAIR(3));
    //////////////////////////////////////////////////////
    while(1){ //new passengers will join the queue indefinitely
        pthread_create(&passenger, NULL, passenger_func, NULL);
        pthread_mutex_lock(&queue_lock);
        queue_size++;
        pthread_mutex_unlock(&queue_lock);
        usleep(rand()%3000000);
    }

	attroff(COLOR_PAIR(3));						
	getch();
	endwin();

    //since the roller coaster never stops, this wont be executed, but in case we create a maximum number of rides, this will be necessary
    pthread_join(car, NULL);
    pthread_mutex_destroy(&board_lock);
    pthread_mutex_destroy(&unboard_lock);
    pthread_mutex_destroy(&queue_lock);
    sem_destroy(&board_sem);
    sem_destroy(&unboard_sem);
    sem_destroy(&full_car);
    sem_destroy(&empty_car);

}