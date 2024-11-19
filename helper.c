#include "helper.h"

volatile unsigned int lowercase = 1;
volatile unsigned int lock = 0;
volatile unsigned int draw = 1;
unsigned char m;
volatile unsigned int fatal_error = 0;
volatile unsigned int cursor = CURSOR_VERTICAL;
volatile unsigned int cursor_on = 0;
signed int cursor_x, cursor_y, chr_x = 0, chr_y = 0;
signed int lastline = 0, modified = 1, offset = 0;

extern volatile unsigned int setuparr[SETUP_NUMKEYS];

unsigned char lines[NUM_LINES+1][LINE_LEN+1];
unsigned char backup[LINE_LEN];

//fkey bitmaps

const unsigned char fkeys_main[] = {
	0x3f, 0xff, 0xf9, 0xff, 0xff, 0xcf, 0xff, 0xfe, 0x7f, 0xff, 0xf3, 0xff, 0xff, 0x9f, 0xff, 0xfc, 
	0x20, 0x0, 0x1, 0x9c, 0xd5, 0xc8, 0x0, 0x0, 0x40, 0x0, 0x2, 0xa1, 0x71, 0x90, 0xba, 0x8c, 
	0x26, 0x14, 0x31, 0x6b, 0xb5, 0xc8, 0x4, 0x0, 0x41, 0xfc, 0x2, 0xaf, 0x76, 0x97, 0xd6, 0xdc, 
	0x29, 0x22, 0x9, 0x6b, 0xb5, 0xc8, 0xe, 0x0, 0x41, 0xfc, 0x2, 0x23, 0x71, 0x91, 0xee, 0xdc, 
	0x2f, 0x7f, 0x39, 0xd, 0xb5, 0xc8, 0x1f, 0x0, 0x40, 0xf8, 0x2, 0xaf, 0x77, 0x97, 0xd6, 0xdc, 
	0x29, 0x22, 0x49, 0x6e, 0xb5, 0x88, 0x3f, 0x80, 0x40, 0x70, 0x2, 0xaf, 0x77, 0x17, 0xba, 0xd8, 
	0x29, 0x14, 0x39, 0x69, 0xd5, 0x8, 0x3f, 0x80, 0x40, 0x20, 0x2, 0xa1, 0x16, 0x10, 0xba, 0xd0, 
	0x20, 0x0, 0x1, 0xff, 0xfe, 0x8, 0x0, 0x0, 0x40, 0x0, 0x3, 0xff, 0xfc, 0x1f, 0xff, 0xe0, 
};

const unsigned char fkeys_onoff[] = {
	0x3f, 0xff, 0xf9, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x20, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x20, 0xc0, 0x1, 0x18, 0x44, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x21, 0x20, 0x1, 0x24, 0x88, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x21, 0x2c, 0x1, 0x25, 0xdc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x21, 0x2a, 0x1, 0x24, 0x88, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x20, 0xca, 0x1, 0x18, 0x88, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x20, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
};

const unsigned char fkeys_plusminus[] = {
	0x3f, 0xff, 0xf9, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x20, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x20, 0x10, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x20, 0x10, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x20, 0x7c, 0x1, 0x3, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x20, 0x10, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x20, 0x10, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x20, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
};

const unsigned char fkeys_darklight[] = {
	0x3f, 0xff, 0xf9, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x20, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x29, 0x32, 0xb9, 0x63, 0x32, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x29, 0x42, 0x91, 0x54, 0xaa, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x29, 0x5b, 0x91, 0x57, 0xb3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x29, 0x4a, 0x91, 0x54, 0xaa, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x2d, 0x32, 0x91, 0x64, 0xaa, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x20, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
};

const unsigned char cursor_vertical[] = {
	0xff, 
	0xff, 
	0xff, 
	0xff, 
	0xff, 
	0xff, 
	0xff, 
};

int strtoi_n(unsigned char *s, unsigned char end) {
    int res = 0;
	int i;

    for (i = 0; s[i] != end; ++i) {
        res = res * 10 + s[i] - '0';
		if (i > 42) { //something is wrong...
			return 0;
		}
	}
 
    return res;
}

void print_com_error(void) {
	cursor_disable();
	PopUpWin(4);
	locate(1, 3);
	Print(LocalizeStringID(100)); //Com ERROR
	locate(1, 5);
	Print(LocalizeStringID(1)); //Press: [EXIT]
	Bdisp_PutDisp_DD();
}

void receive_timer(void) {
	unsigned char chr[BUF_SIZE + 1], tmp;
    unsigned char escbuf[ESCBUF_SIZE];
	unsigned int l, i, p, a, b, m, seqstart;

	if (lock == 1) {
		return;
	}

	lock = 1;

	if (Comm_Spy0thByte(&chr[0]) == 6) { //is connection terminated?
		fatal_error = 1;
		draw = 0;
		print_com_error();
		return;
	}

	while (Comm_Spy0thByte(&chr[0]) == 0) {
	    Comm_ReadOneByte(&chr[0]);

        if (chr_x >= LINE_LEN) {
            chr_x -= LINE_LEN;

			chr_y++; lastline++;
        }

        if (chr_y >= NUM_LINES) {
            chr_y--; lastline--;
			
            a = 0;

            while (a < (NUM_LINES-1)) {
                memcpy(lines[a], lines[a+1], LINE_LEN);
                a++;
            }          
            memset(lines[a], 0, LINE_LEN);
        }
        
		if (chr[0] == 0x0d) { //CR
			chr_x = 0;
		}
		else if (chr[0] == 0x0a) { //LF
			chr_y++; lastline++;
		}
		else if (chr[0] == 0x09) { //HT; I don't have time for this...
			lines[chr_y][chr_x++] = ' '; 	
		}
		else if (chr[0] == 0x07) { //BELL, nothing
		}
        else if (setuparr[SETUP_KEY_PROCESC] == SETUP_PROCESC_ON && chr[0] == 0x1b) { //ESCAPE, oh no
			i = 0;
            while (i < ESCBUF_SIZE && (Comm_Spy0thByte(&tmp) != 0x18)) {
				Comm_ReadOneByte(&escbuf[i]);
				if (escbuf[i] == '[') seqstart = i;
				if (isalpha(escbuf[i])) break;
			}
			if (escbuf[i] == 'K') {
				memset(&lines[chr_y][chr_x], 0, LINE_LEN - 1 - chr_x); 
			}
			if (escbuf[i] == 'H' && (seqstart+1 == i)) {
				chr_x = 0;
				chr_y = (chr_y >= 7) ? chr_y - 7 : 0;
			}
        }
		else if (chr[0] == 0x08) { //BS
			if (chr_y+chr_x > 0) { //only if not at 0;0
				chr_x--;
				if (chr_x < 0) {
					chr_y--; lastline--;
					chr_x = strlen(lines[chr_y])-1;
				}
				lines[chr_y][chr_x] = ' ';
			}
		}
		else {
            lines[chr_y][chr_x++] = chr[0];
		}

		if (offset > lastline) {
			offset = lastline;
		}	
	
		modified = 1;
	}

	lock = 0;
}

void cursor_timer(void) {
	if (!draw || !cursor_on) return;

	if (GetShiftAlphaState() & 0x0c && GetShiftAlphaState() & 0x80) {
		SetShiftAlphaState(lowercase?0x84:0x88);
		cursor_set(lowercase?CURSOR_ALPHA_L:CURSOR_ALPHA_U);
	}
	else if (GetShiftAlphaState() & 0x0c && !(GetShiftAlphaState() & 0x80)) {
		SetShiftAlphaState(lowercase?0x04:0x08);
		cursor_set(lowercase?CURSOR_ALPHA_L:CURSOR_ALPHA_U);
	}
}

void display_timer(void) {
	int i, l = 1, o;

	if (modified) {
		if (lock || !draw) {
			return;
		}

		Bdisp_AllClr_DDVRAM();	
		
		i = (lastline-offset);

        l = 7;
        if ((lastline-offset)-7 < 0) {
            l+=((lastline-offset)-7);
        }
        o = l;
    
        while (i > 0 && i > ((lastline-offset)-7)) {
            locate(1, l--);
            Print(lines[i]);
            i--;
        }

        locate(strlen(lines[lastline-offset])+1, o);
		
		print_fkey_row();	

		if (setuparr[SETUP_KEY_THEME] == SETUP_THEME_DARK) {
			Bdisp_AreaReverseVRAM(0, 0, 127, 63);
		}	

		Bdisp_PutDisp_DD();
		modified = 0;
	}
}

void draw_bitmap_VRAM(unsigned char *bitmap, unsigned short width, unsigned short height, unsigned short x, unsigned short y) {
	DISPGRAPH img;
	img.WriteModify = IMB_WRITEMODIFY_NORMAL;
	img.x = x;
	img.y = y;
	img.WriteKind = IMB_WRITEKIND_OVER;
	img.GraphData.width = width;
	img.GraphData.height = height;
	img.GraphData.pBitmap = bitmap;
	Bdisp_WriteGraph_VRAM(&img);
}

void print_fkey_row(void) {
	draw_bitmap_VRAM(fkeys_main, 128, 8, 0, 56);
}

int handle_key(unsigned int key) {
	unsigned char send[16];
	unsigned int n = 0, i;
	
	send[0] = 0;

	if ((key >= 0x20 && key <= 0x40) || (key >= 0x5b && key <= 0x60) || (key >= 0x7b && key <= 0x7e)) {
		send[0] = key & 0xFF;
	}
	else if (key == KEY_CTRL_F1) {
		lowercase = !lowercase;
	}
	else if (key == KEY_CTRL_F2) {
		draw = 0;
		send[0] = asciidlg();
		draw = 1;
	}
	else if (key == KEY_CTRL_F3) {
		if (offset < (lastline-7)) offset++;
		modified = 1;
	}
	else if (key == KEY_CTRL_F4) {
		if (offset > 0) offset--;
		modified = 1;
	}
	else if (key == KEY_CTRL_F5) {
		draw = 0;
		helpdlg();
		draw = 1;
	}
	else if (key == KEY_CTRL_SETUP) {
		draw = 0;
		setupmenu();
		draw = 1;
	}
	else if (key >= 0x41 && key <= 0x5a) {
		if (lowercase) {
			send[0] = key + ('a' - 'A');
		}
		else {
			send[0] = key;
		}
	}
	else if (key >= 0x61 && key <= 0x7a) {
		if (lowercase) {
			send[0] = key;
		}
		else {
			send[0] = key - ('a' - 'A');
		}
	}
	else if (key == KEY_CTRL_EXE) {
		send[0] = 0x0a; //LF
	}
	else if (key == KEY_CTRL_AC) {
		send[0] = 0x03; //ETX
	}
	else if (key == KEY_CTRL_EXIT) {
		send[0] = 0x04; //EOT
	}
	else if (key == KEY_CTRL_DEL) {
		send[0] = 0x08; //BS
	}
	else if (key == KEY_CHAR_MINUS) {
		send[0] = '-';
	}
	else if (key == KEY_CHAR_DIV) {
		send[0] = '/';
	}	

	else if (key == KEY_CTRL_F6) {
		return 1;
	}

	if (send[0] != 0) {
		i = 0;
		do {
			Comm_TransmitOneByte(send[i]);
		} while (i++ < n);
	}
	return 0;
}

unsigned char asciidlg(void) {
	int i, j, k, l;
	unsigned int key;
	unsigned char c[2] = {0,0};
	unsigned int settings[4];

	SaveDisp(1);

	cursor_disable();
	Cursor_GetSettings(settings);

	i = 0;
	j = 0;

	while (1) {
		Bdisp_AllClr_DDVRAM();
	
		locate(1, 1);
		Print((unsigned char *)"Choose ASCII char");
	
		for (k = 0; k < ASC_ROWS; k++) {
			for (l = 0; l < ASC_COLS; l++) {
				c[0] = 0x20 + l + k*ASC_COLS;
				locate(l+1, k+2);
				PrintC(c);
			}
		}

		c[0] = 0x20 + j + i*ASC_COLS;
		locate(j+1, i+2);
		PrintRevC(c);

		if (setuparr[SETUP_KEY_THEME] == SETUP_THEME_DARK) {
			Bdisp_AreaReverseVRAM(0, 0, 127, 63);
		}

		GetKey(&key);
		
		if (key == KEY_CTRL_EXE) {
			RestoreDisp(1);
			locate(settings[0]+1, settings[1]+1);
			cursor_enable();
			return 0x20 + j + i*ASC_COLS;
		}
		if (key == KEY_CTRL_EXIT) {
			RestoreDisp(1);
			locate(settings[0]+1, settings[1]+1);
			cursor_enable();
			return 0;
		}
		if (key == KEY_CTRL_LEFT && j > 0) {
			locate(j+1, i+2);
			PrintC(c);
			j--;
		}
		if (key == KEY_CTRL_RIGHT && j < ASC_COLS-1) {
			locate(j+1, i+2);
			PrintC(c);
			j++;
		}
		if (key == KEY_CTRL_UP && i > 0) {
			locate(j+1, i+2);
			PrintC(c);
			i--;
		}
		if (key == KEY_CTRL_DOWN && i < ASC_ROWS-1) {
			locate(j+1, i+2);
			PrintC(c);
			i++;
		}
	}
}

void helpdlg(void) {
	unsigned int key;
	unsigned int settings[4];

	SaveDisp(1);
	cursor_disable();
	Cursor_GetSettings(settings);

	Bdisp_AllClr_DDVRAM();

	locate(1, 1);
	Print((unsigned char *)"Key Help");

	locate(1, 2);
	Print((unsigned char *)"[AC]: ^C");

	locate(1, 3);
	Print((unsigned char *)"[EXIT]: ^D");

	locate(1, 4);
	Print((unsigned char *)"[\xB9]: /");

	locate(1, 6);
	Print((unsigned char *)"[SET UP]: Setup");

	if (setuparr[SETUP_KEY_THEME] == SETUP_THEME_DARK) {
		Bdisp_AreaReverseVRAM(0, 0, 127, 63);
	}

	GetKey(&key);

	RestoreDisp(1);
	locate(settings[0]+1, settings[1]+1);
	cursor_enable();
}

void setupmenu(void) {
	unsigned int i, baud, hadhelp, escape, sel = 0;
	unsigned int key;
	unsigned int settings[4];

	SaveDisp(1);
	cursor_disable();
	Cursor_GetSettings(settings);

	setup_read(setuparr, SETUP_NUMKEYS);

	while (1) {
		Bdisp_AllClr_DDVRAM();
	
		if (SETUP_NUMKEYS > 7) { //not yet
			return;
		}

		locate(1, 1);
		Print((unsigned char *)"ESC Sequence:");

		switch (setuparr[SETUP_KEY_PROCESC]) {
			case SETUP_PROCESC_ON:
				Print((unsigned char *)"On");
				break;
			case SETUP_PROCESC_OFF:
				Print((unsigned char *)"Off");
				break;
			default:
				setuparr[SETUP_KEY_PROCESC] = SETUP_PROCESC_ON;
				Print((unsigned char *)"On");
				break;
		}

		locate(1, 2);
		Print((unsigned char *)"Color Theme :");

		switch (setuparr[SETUP_KEY_THEME]) {
			case SETUP_THEME_LIGHT:
				Print((unsigned char *)"Light");
				break;
			case SETUP_THEME_DARK:
				Print((unsigned char *)"Dark");
				break;
			default:
				setuparr[SETUP_KEY_THEME] = SETUP_THEME_LIGHT;
				Print((unsigned char *)"Light");
				break;
		}

		Bdisp_AreaReverseVRAM(0, sel*8, 127, sel*8+7);

		switch (sel) {
			case SETUP_KEY_PROCESC:
				draw_bitmap_VRAM(fkeys_onoff, 128, 8, 0, 56);
				break;
			case SETUP_KEY_THEME:
				draw_bitmap_VRAM(fkeys_darklight, 128, 8, 0, 56);
				break;
			default:
				break;
		}

		if (setuparr[SETUP_KEY_THEME] == SETUP_THEME_DARK) {
			Bdisp_AreaReverseVRAM(0, 0, 127, 63);
		}

		GetKey(&key);
		if (key == KEY_CTRL_EXIT || key == KEY_CTRL_EXE) {
			break;
		}
		if (key == KEY_CTRL_UP && sel > 0) sel--; 
		if (key == KEY_CTRL_DOWN && sel < (SETUP_NUMPRINTABLEKEYS - 1)) sel++; 

		if (key == KEY_CTRL_F1) {
			switch(sel) {
				case SETUP_KEY_PROCESC:
					setuparr[SETUP_KEY_PROCESC] = SETUP_PROCESC_ON;
					break;
				case SETUP_KEY_THEME:
					setuparr[SETUP_KEY_THEME] = SETUP_THEME_LIGHT;
					break;
				default:
					break;
			}
		}
		if (key == KEY_CTRL_F2) {
			switch(sel) {
				case SETUP_KEY_PROCESC:
					setuparr[SETUP_KEY_PROCESC] = SETUP_PROCESC_OFF;
					break;
				case SETUP_KEY_THEME:
					setuparr[SETUP_KEY_THEME] = SETUP_THEME_DARK;
					break;
				default:
					break;
			}
		}
		
		setup_write(setuparr, SETUP_NUMKEYS);
		modified = 1;
	}
	
	RestoreDisp(1);
	locate(settings[0]+1, settings[1]+1);
	cursor_enable();
}

void cursor_enable(void) {
	Cursor_SetFlashOn(CURSOR_VERTICAL);
	cursor_on = 1;
}

void cursor_disable(void) {
	Cursor_SetFlashOff();
	cursor_on = 0;
}

void cursor_set(unsigned int cursor) {
	Cursor_SetFlashOn(cursor);
}
