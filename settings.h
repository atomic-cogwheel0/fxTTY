#ifndef SETTINGS_H
#define SETTINGS_H

#include "fxlib.h"

#define SETUP_NUMKEYS 3

#define SETUP_NUMPRINTABLEKEYS 2

#define SETUP_KEY_PROCESC 0
#define SETUP_KEY_THEME 1
#define SETUP_KEY_KNOWLEDGE 2

void setup_read(unsigned int *setup, int keys);
void setup_write(unsigned int *setup, int keys);

#endif