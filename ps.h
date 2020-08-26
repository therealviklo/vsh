#pragma once
#include <stdbool.h>

typedef struct {
	int ret;
	enum {
		ESE_SUCCESS,
		ESE_INVALID,
		ESE_MEM,
		ESE_ABNORMALEXIT
	} error;
} ExecuteStatus;

ExecuteStatus execute(const char* str);
// True om den lyckades
bool currDir(char* buffer, size_t size);
// True om den lyckades
bool currUser(char* buffer, size_t size);