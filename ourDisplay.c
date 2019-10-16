// ******************* Includes ******************* 

#include <stdio.h>
#include "display.h"


// ******************* Global variables ******************* 

unsigned char* port1 = (unsigned char *) 0x40000000;
unsigned char* port2 = (unsigned char *) 0x42000000;
unsigned char* port3 = (unsigned char *) 0x44000000;
unsigned char* port4 = (unsigned char *) 0x46000000;


Board initBoard()
{
	Board board = malloc(sizeof(*Board));
}