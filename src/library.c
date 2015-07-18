
/*
 *	CS1550 - Project 1 _ Functions Implementation  
 *	Author 		: 	You Zhou
 *	Pitt Email 	: 	yoz13@pitt.edu
 *	People Soft # 	: 	3729901
 */
 
#include "library.h"
#include "iso_font.h"

static int fd;
static struct fb_var_screeninfo var_info;
static struct fb_fix_screeninfo fix_info; 
static char* addr;
static struct termios oldT, newT;
int X_BOUND;
int Y_BOUND;

void init_graphics()
{
	clear_screen();
	
	fd = open("/dev/fb0", O_RDWR);
	if(fd == -1)
	{
		printf("Can't open the device: %s : %s\n", "/dev/fb0", strerror(errno));
		exit(-1);
	}
	
	if(ioctl(fd, FBIOGET_VSCREENINFO, &var_info) == -1)
	{
		printf("FBIOGET_VSCREENINFO failed: %s\n", strerror(errno));
		exit(-1);
	}
	X_BOUND = var_info.xres_virtual;
	Y_BOUND = var_info.yres_virtual;
	
	if(ioctl(fd, FBIOGET_FSCREENINFO, &fix_info) == -1)
	{
		printf("FBIOGET_FSCREENINFO failed: %s\n", strerror(errno));
		exit(-1);
	}
	
	addr = (char*) mmap(NULL, Y_BOUND*fix_info.line_length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(addr == MAP_FAILED)
	{
		printf("MEMORY MAPPING failed: %s\n", strerror(errno));
		exit(-1);
	}	
	
	if(ioctl(0, TCGETS, &oldT) == -1)
	{
		printf("GETTING OLD TERMINAL ATTRIBUTES failed: %s\n", strerror(errno));
		exit(-1);
	}
	newT = oldT;
	newT.c_lflag &= (~ICANON & ~ECHO);
	
	if(ioctl(0, TCSETS, &newT) == -1)
	{
		printf("SETTING NEW TERMINAL ATTRIBUTES failed: %s\n", strerror(errno));
		exit(-1);
	}
}

void exit_graphics()
{
	clear_screen();
	if(ioctl(0, TCSETS, &oldT) == -1)
	{
		printf("SETTING BACK OLD TERMINAL ATTRIBUTES failed: %s\n", strerror(errno));
		exit(-1);
	}
	if(munmap(addr, Y_BOUND*fix_info.line_length))
	{
		printf("MEMORY UNMAPPING failed: %s\n", strerror(errno));
		exit(-1);
	}
}

void clear_screen()
{
	if(write(1, "\033[2J", 7) == -1)
	{
		printf("CLEARING SCREEN failed: %s\n", strerror(errno));
		exit(-1);
	}
}

char getkey()
{
	fd_set result;
	FD_ZERO(&result);
	FD_SET(0, &result);
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	char c;
	if(select(1, &result, NULL, NULL, &tv))
	{
		if(read(0, &c, 1))
			return c;
		else
		{
			printf("READING CHARACTER failed: %s\n", strerror(errno));
			exit(-1);
		}
	}
	return '\0';
}

void sleep_ms(long ms)
{
	struct timespec tim1, tim2;
	tim1.tv_sec = 0;
	tim2.tv_nsec = ms*1000000;
	if(nanosleep(&tim1, &tim2) == -1)
	{
		printf("NANOSLEEP failed: %s\n", strerror(errno));
		exit(-1);
	}
}

void draw_pixel(int x, int y, color_t color)
{
	*((color_t*)(addr + x*var_info.bits_per_pixel/8 + y*fix_info.line_length)) = color;
}

void draw_rect(int x1, int y1, int width, int height, color_t color)
{
	const int L = width;
	while(width-->0)
	{
		draw_pixel(x1 + width, y1,          color);
		draw_pixel(x1 + width, y1 + height, color);
	}
	while(height-->0)
	{
		draw_pixel(x1,     y1 + height, color);
		draw_pixel(x1 + L, y1 + height, color);
	}
}

void draw_text(int x, int y, const char* text, color_t color)
{
	unsigned char ch;
	for(x, text ; ch = *(text) ; x+=8, text++)
		draw_char(x, y, iso_font + ch * 16, color);
}

void draw_char(int x, int y, unsigned char* ch, color_t color)
{
	int i, slot;
	unsigned char c;
	for(i = 0 ; i < 16 ; i++, y++)
		for(slot = 0, c = *(ch + i) ; slot < 8 ; slot++, c = c >> 1)
			if(c & 0x01)
				draw_pixel(x + slot, y, color);
}
