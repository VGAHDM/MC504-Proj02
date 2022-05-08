#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int waddstr_trunc(WINDOW *win, const char *str){
  int cur_x, max_x, dummy;
  getyx(win, dummy, cur_x);
  getmaxyx(win, dummy, max_x);
  int w=max_x - cur_x;
  if (w <= 0) return 0;
  char *str2 = strndup(str, w);
  if (str2 == NULL) return 1;
  int rv = waddstr(win, str2);
  free(str2);
  return rv;
}

void negative_print(WINDOW *win, const char *str, int row, int col){
	if(strlen(str)<= (-1*col)) return;
	char str2[strlen(str)+col];
	sprintf(str2, "%s", str+(-1*col));
	move(row,0);
	waddstr_trunc(win,str2);
	//free(str2);
}
void run(WINDOW *win, int queue){
	int max_row,max_col;
	char bar[]="+------------+";	
	char msg[]="|  Leaving   |";		
	getmaxyx(win,max_row,max_col);
	
	for(int i = 0; i<max_col; i++){
		clear();
		mvprintw(1,(max_col-strlen(bar))/2,"%s",bar);
		mvprintw(2,(max_col-strlen(msg))/2,"%s",msg);
		mvprintw(3,(max_col-strlen(bar))/2,"%s",bar);
	
		//mvprintw(1,i,"   _ \\o/ _     _   _     _     _   _     _     __");
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

		move(11,0);
		attron(A_UNDERLINE);
		waddstr_trunc(win,"Entrada");
		attroff(A_UNDERLINE);
		move(11,max_col-5);
		attron(A_UNDERLINE);
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
		refresh();
		if(!i) usleep(1000000);
		if(50000-(i*1000)>12000)
		usleep(60000-(i*1000));
		else usleep(12000);
	}
}

void board(WINDOW *win, int queue, int boarders){
	int max_row,max_col;
	char bar[]="+------------+";	
	char msg[]="|  Boarding  |";		
	getmaxyx(win,max_row,max_col);
	
	clear();
	mvprintw(1,(max_col-strlen(bar))/2,"%s",bar);
	mvprintw(2,(max_col-strlen(msg))/2,"%s",msg);
	mvprintw(3,(max_col-strlen(bar))/2,"%s",bar);
	
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
	move(11,0);
	attron(A_UNDERLINE);
	waddstr_trunc(win,"Entrada");
	attroff(A_UNDERLINE);
	move(11,max_col-5);
	attron(A_UNDERLINE);
	waddstr_trunc(win,"Saida");
	attroff(A_UNDERLINE);
	move(11,(max_col)/2);
	waddstr_trunc(win,"|");
	move(12,max_col/2);
	waddstr_trunc(win,"|");
	for(int i = 1; i <= queue; i++){
		move(12,(max_col/2) - 3*i);
		waddstr_trunc(win," o ");
		move(13,(max_col/2) - 3*i);
		waddstr_trunc(win,"/|\\");
		move(14,(max_col/2) - 3*i);
		waddstr_trunc(win,"/ \\");
	}
	move(13,max_col/2);
	waddstr_trunc(win,"|");
	move(14,max_col/2);
	waddstr_trunc(win,"|");
	move(15,0);
	waddstr_trunc(win,"__________________________________________________________________________________________________________________________________________________________________");
	move(15,(max_col)/2);
	waddstr_trunc(win,"|");
	refresh();
	usleep(500000);
	
}

void unboard(WINDOW *win, int queue){
	int max_row,max_col, car_len = strlen(" | |___| |___| |-| |___| |___| |-| |___| |___|   )");
	char bar[]="+------------+";	
	char msg[]="|  Arriving  |";		
	getmaxyx(win,max_row,max_col);

	usleep(100000);
	for(int i = (-1*car_len); i <= 0; i++){
		clear();
		mvprintw(1,(max_col-strlen(bar))/2,"%s",bar);
		mvprintw(2,(max_col-strlen(msg))/2,"%s",msg);
		mvprintw(3,(max_col-strlen(bar))/2,"%s",bar);

		negative_print(win, " __ \\o/ _ \\o/ _   _ \\o/ _ \\o/ _   _ \\o/ _ \\o/ __", 5, i);
		negative_print(win," | |_|_| |_|_| | | |_|_| |_|_| | | |_|_| |_|_|  \\ ", 6, i);
		negative_print(win," | |___| |___| |-| |___| |___| |-| |___| |___|   )", 7, i);
		negative_print(win," |_____________| |_____________| |______________/", 8, i);
		move(9,0);
		waddstr_trunc(win,"----------------------------------------------------------------------------------------------------------------------------------------------------------");
		move(10,0);
		waddstr_trunc(win,"__________________________________________________________________________________________________________________________________________________________________");

		move(11,0);
		attron(A_UNDERLINE);
		waddstr_trunc(win,"Entrada");
		attroff(A_UNDERLINE);
		move(11,max_col-5);
		attron(A_UNDERLINE);
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
		refresh();

		if(50000 + (i*1000)>20000)
		usleep(60000+(i*1000));
		else usleep(20000);
	}
}


int main(){	
	int max_row,max_col;	
	initscr();			
	getmaxyx(stdscr,max_row,max_col);	
	
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLUE);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
	init_pair(4, COLOR_RED, COLOR_MAGENTA);
	clear();
	attron(COLOR_PAIR(1));
	
	board(stdscr,8,0);
	board(stdscr,8,1);
	board(stdscr,8,2);
	board(stdscr,7,3);
	board(stdscr,6,4);
	board(stdscr,5,5);
	board(stdscr,4,6);
	run(stdscr,0);
	unboard(stdscr,0);
	
	attroff(COLOR_PAIR(1));			
	refresh();			
	getch();
	
	endwin();			

	return 0;
}
