#ifndef SYSCALL_H
#define SYSCALL_H

// 0000 0000 0010 0000
// ^ ^1 stopbit^  ^	        
// no parity   |  baud
//             USB

#define COMM_OPEN_USB_MASK 0x20

#define INBUF_SIZE 16
#define OUTBUF_SIZE 16

int KeyBoard_GetKeyWait(int *column, int *row, int type_of_waiting, int timeout_period, int menu, unsigned short *keycode);
int KeyBoard_ClrBuffer(void);

#define IN_MODE_DEC 0
#define IN_MODE_HEX 1

int InputNumber(unsigned char *heading, int maxlen, int mode);
int InputString(unsigned char *buffer, unsigned char *heading, int maxlen);

int LongToAscHex(int value, char *dest, int digits);

// if params & 0xF0 == 0x20 then USB

int Comm_Open(unsigned short params);
int Comm_GetCurrentSelector(void);
int Comm_TransmitOneByte(unsigned char byte);
int Comm_Close(int mode);
int Comm_ReadOneByte(unsigned char *result);
int Comm_WaitForAndReadNBytes(char *buffer, int cnt);
int Comm_TransmitNBytes(unsigned char *bytes, int cnt);
int Comm_IsValidPacketAvailable(unsigned char *result);

//timeout: min 25 ms
//is_rec 1: receive buffer check
//ret 6: timed out

#define WAIT_RECEIVE 1
#define WAIT_ETIMEOUT 6
#define MIN_WAIT_MS 50

int Comm_WaitForAnyBuffer(int timeout_ms, int is_rec, int *t);

//ret: 0x18 no byte avail.

int Comm_Spy0thByte(unsigned char *result);

#define CURSOR_VERTICAL 	0
#define CURSOR_SHIFT 		1	
#define CURSOR_ALPHA_U  	3
#define CURSOR_ALPHA_L		4

void Cursor_SetFlashOn(char style);
void Cursor_SetFlashOff(void);

//settings is an array of 4

unsigned int Cursor_GetSettings(unsigned int *settings);

unsigned char *LocalizeStringID(int id);

int GetShiftAlphaState(void);
int SetShiftAlphaState(int state);

int GetAutoPowerOffTime(void);
int SetAutoPowerOffTime(int t);

#endif
