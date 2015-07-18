/*
 *	CS1550 - Project 1 _ Functions Implementation  
 *	Author 		: 	You Zhou
 *	Pitt Email 	: 	yoz13@pitt.edu
 *	People Soft # 	: 	3729901
 */

#include "library.h"
#define MIN(a,b) ((a<b) ? a : b)
#define MAX(a,b) ((a<b) ? b : a)
#define COLOR(r, g, b) ((((r << 6) + g) << 5) + b)


int main(int argc, char** argv)
{
	init_graphics();
	const char* RULES = "use wasd to move Rona Shan and q to quit.";
	const char* CHARSEQUENCE = "Rona Shan"; 
	const int XWALL = X_BOUND-100, YWALL = Y_BOUND-120;
	int x = 100, y = 100;
	char key;
	do
	{
		draw_rect(20, 20, X_BOUND-30, Y_BOUND-30, COLOR(0, 0, 20));//(RGB_RED, RGB_GREEN, RGB_BLUE)
		draw_text(25, 25, RULES, COLOR(0, 20, 0));
		
		draw_text(x, y, CHARSEQUENCE, COLOR(0, 0, 0));
		key = getkey();
		if(key == 'w')
			y = MAX(60, y-10);
		else if(key == 's')
			y = MIN(y+10, YWALL);
		else if(key == 'a')
			x = MAX(60, x-10);
		else if(key == 'd')
			x = MIN(x+10, XWALL);
		draw_text(x, y, CHARSEQUENCE, COLOR(20, 0, 0));
		
		sleep_ms(2000);
	} 	
	while(key != 'q');
	exit_graphics();
	return 0;
}
