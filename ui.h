#ifndef UI_H
#define UI_H

#include "fxlib.h"
#include "syscall.h"
#include "settings.h"
#include <ctype.h>
#include <string.h>

#define BUF_SIZE 32
#define NUM_LINES 32
#define LINE_LEN 21

#define ESCBUF_SIZE 64

#define REC_TMR ID_USER_TIMER1
#define DISP_TMR ID_USER_TIMER2
#define CURSOR_TMR ID_USER_TIMER3

#define SETUP_PROCESC_ON 0
#define SETUP_PROCESC_OFF 1

#define SETUP_KNOWLEDGE_NONE 0
#define SETUP_KNOWLEDGE_KEYHELP 1

#define SETUP_THEME_LIGHT 0
#define SETUP_THEME_DARK 1
 
void receive_timer(void);
void cursor_timer(void);
void display_timer(void);

int handle_key(unsigned int key);

void draw_bitmap_VRAM(unsigned char *bitmap, unsigned short width, unsigned short height, unsigned short x, unsigned short y);
void print_com_error(void);
void print_fkey_row(void);
unsigned char asciidlg(void);
unsigned char asciidlg_custom(void);
void helpdlg(void);
void setupmenu(void);

void cursor_enable();
void cursor_disable();

void cursor_set(unsigned int cursor);

#define ASC_ROWS 6
#define ASC_COLS 16

#endif