#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


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

void draw_queue(WINDOW *win, int queue){
	int max_row,max_col;		
	getmaxyx(win,max_row,max_col);

	for(int i = 1; i <= queue; i++){
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

void arrive(WINDOW *win, int queue){
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

		draw_queue(win, queue);

		refresh();

		if(50000 + (i*1000)>20000)
		usleep(60000+(i*1000));
		else usleep(20000);
	}
}


void run(WINDOW *win, int queue){
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

 		draw_queue(win, queue);
		
		refresh();
		if(!i) usleep(1000000);
		if(50000-(i*1000)>12000)
		usleep(60000-(i*1000));
		else usleep(12000);
	}

	usleep(300000);
	arrive(win,queue);
}

void board(WINDOW *win, int queue, int boarders){
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
	

	draw_queue(win, queue);

	refresh();
	usleep(500000);
	
}

void unboard(WINDOW *win, int queue, int boarders){
	int max_row,max_col;		
	getmaxyx(win,max_row,max_col);
	
	for(int i = max_col/2; i<max_col-5;i++){

		clear();
		draw_header(win, 4); 

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
		move(10,((max_col)/2)+5);
		waddstr_trunc(win,"|  |");

		draw_queue(win, queue);

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

int main(){	
	int max_row,max_col;	
	initscr();			
	getmaxyx(stdscr,max_row,max_col);	
	
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	clear();
	attron(COLOR_PAIR(3));
	
	board(stdscr,10,0);
	board(stdscr,9,1);
	board(stdscr,8,2);
	board(stdscr,7,3);
	board(stdscr,6,4);
	board(stdscr,5,5);
	board(stdscr,4,6);
	run(stdscr,4);
	unboard(stdscr,4,5);
	unboard(stdscr,4,4);
	unboard(stdscr,4,3);

	attroff(COLOR_PAIR(3));			
	refresh();			
	getch();
	
	endwin();			

	return 0;
}
