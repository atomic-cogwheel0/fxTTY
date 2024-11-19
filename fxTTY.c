#include "fxlib.h"
#include "syscall.h"
#include "timer.h"
#include "ui.h"
#include <string.h>
#include <stdlib.h>

extern volatile unsigned int fatal_error;
extern volatile unsigned int setuparr[SETUP_NUMKEYS];

extern volatile unsigned int lowercase;

void quit(void) {
	Comm_Close(1);
	KillTimer(REC_TMR);
	KillTimer(DISP_TMR);
	KillTimer(CURSOR_TMR);
}

int AddIn_main(int isAppli, unsigned short OptionNum) {
    unsigned int key = 0, i;
	unsigned char byte;

    Bdisp_AllClr_DDVRAM();

	setup_read(setuparr, SETUP_NUMKEYS);

	if (setuparr[SETUP_KEY_KNOWLEDGE] == SETUP_KNOWLEDGE_NONE) {
		helpdlg();	
	}

	setuparr[SETUP_KEY_KNOWLEDGE] = SETUP_KNOWLEDGE_KEYHELP;

	setup_write(setuparr, SETUP_NUMKEYS);

	Comm_Open(0 | (COMM_OPEN_USB_MASK));
	
	SetQuitHandler(quit);	

	if (Comm_GetCurrentSelector() == 1) {   
		SetTimer(REC_TMR, 50, receive_timer); //schedule receiver
		SetTimer(CURSOR_TMR, 25, cursor_timer); //schedule cursor flasher
		SetTimer(DISP_TMR, 100, display_timer); //schedule display
		
		cursor_enable();

		SetShiftAlphaState(lowercase?0x84:0x88);

		while (1) {
			cursor_timer();
			GetKey(&key);
			if (!fatal_error) {
				if (handle_key(key)) {
					break;
				}
			}
			else {
				if (key == KEY_CTRL_EXIT) {
					break;
				}
			}
		}
	}
    else {
		print_com_error();
		while (1) {
			GetKey(&key);
			if (key == KEY_CTRL_EXIT) {
				break;
			}
		}
    }

    return 1;
}


//CASIO stuff


//**************                 Notice!                      ****************
//**************                                              ****************
//**************  Please do not change the following source.  ****************


#pragma section _BR_Size
unsigned long BR_Size;
#pragma section


#pragma section _TOP

int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}

#pragma section

