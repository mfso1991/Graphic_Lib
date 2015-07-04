
/*
 *	CS1550 - Project 1 _ User Interface  
 *	Author 			: 		You Zhou
 *	Pitt Email 		: 		yoz13@pitt.edu
 *	People Soft # 	: 		3729901
 */

#include <linux/fb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <termios.h>
#include <sys/select.h> 
#include <sys/time.h> 
#include <unistd.h> 

/*
 *	
 *
 *
 */
extern int X_BOUND, Y_BOUND;

/*
 *	
 *
 *
 */
typedef unsigned short color_t; 

/*
 *	
 *
 *
 */
void init_graphics();

/*
 *	
 *
 *
 */
void exit_graphics();

/*
 *	
 *
 *
 */
void clear_screen();

/*
 *	
 *
 *
 */
char getkey();

/*
 *	
 *
 *
 */
void sleep_ms(long ms);

/*
 *	
 *
 *
 */
void draw_pixel(int x, int y, color_t color);

/*
 *	
 *
 *
 */
void draw_rect(int x1, int y1, int width, int height, color_t color);

/*
 *	
 *
 *
 */
void draw_text(int x, int y, const char* text, color_t color);

/*
 *	
 *
 *
 */
void draw_char(int x, int y, unsigned char* ch, color_t color);
