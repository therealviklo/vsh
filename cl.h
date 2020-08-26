#pragma once
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
	char* path;
	char** argv;
} CL;

CL* CLcreate(const char* str);
void CLfree(CL* cl);