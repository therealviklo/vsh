#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ps.h"
#include "charsizes.h"
#include "conv.h"

typedef struct LSSnode {
	char c[4];
} LSSnode;

typedef struct {
	LSSnode* arr;
	size_t size;
	size_t pos;
	CursorPos beg;
} LSS;

LSS* LSScreate(void);
void LSSfree(LSS* lss);
void LSSadd(LSS* lss, char c[4]);
void LSSpop(LSS* lss);
void LSSreprint(LSS* lss);
char* LSSgetstr(LSS* lss);
char* LSSrelease(LSS* lss);
void LSSleft(LSS* lss);
void LSSright(LSS* lss);
void LSSgotoend(LSS* lss);