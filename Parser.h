#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFFSIZE 8192

// Parse message and return whether it was destroy board message and therefore loop should end
int parseMessage(char* input);

#endif