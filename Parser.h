#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <string.h>
#include "Display.h"

#define BUFFSIZE 8192

// Parses message and returns whether it was destroy board message and therefore loop should end
int parseMessage(char* input);

#endif
