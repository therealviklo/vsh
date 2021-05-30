#pragma once
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* str;
	size_t size;
} SS;

SS* SScreate(void);
void SSfree(SS* ss);
void SSadd(SS* ss, char c);
void SSpop(SS* ss);
char* SSrelease(SS* ss);