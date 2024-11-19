#include "settings.h"

volatile unsigned int setuparr[SETUP_NUMKEYS];

unsigned int AscToLongHex(unsigned char *hex, int n) {
    unsigned int val = 0;
	unsigned char byte;
	unsigned int i = 0;

    while (i++ < n) {
        byte = *hex++; 
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <= 'f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <= 'F') byte = byte - 'A' + 10;    
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

void setup_read(unsigned int setup[SETUP_NUMKEYS], int keys) {
	int handle = 0;
	unsigned int key;
	unsigned int pos = 0;
	int i;
	unsigned char buf[5];

	memset(buf, 0, 5);

	handle = Bfile_OpenMainMemory("SETUP");

	for (i = 0; i < keys; i++) {
		if (Bfile_ReadFile(handle, buf, 4, pos) < 0) {
			memset(buf, 0, 4);
		}
		pos += 4;
  		setup[i] = AscToLongHex(buf, 4);
	}
	
	Bfile_CloseFile(handle);
}

void setup_write(unsigned int setup[SETUP_NUMKEYS], int keys) {
	int handle = 0;
	int i;
	unsigned char buf[5];

	memset(buf, 0, 5);

	Bfile_DeleteMainMemory("SETUP");		
	Bfile_CreateMainMemory("SETUP");
	handle = Bfile_OpenMainMemory("SETUP");

	for (i = 0; i < keys; i++) {
		LongToAscHex(setup[i], buf, 4);
		Bfile_WriteFile(handle, buf, 4);
	}

	Bfile_CloseFile(handle);
}