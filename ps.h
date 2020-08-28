#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef VSH_LINUX
#define CLR_WOB 0
#define CLR_DARKRED 31
#define CLR_DARKGREEN 32
#define CLR_LIGHTRED 91
#define CLR_LIGHTGREEN 92
#define CLR_LIGHTBLUE 94
#define CLR_LIGHTCYAN 96
typedef uint8_t Colour;
#endif /* VHS_LINUX */
#ifdef VSH_WINDOWS
#define CLR_WOB (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define CLR_DARKRED (FOREGROUND_RED)
#define CLR_DARKGREEN (FOREGROUND_GREEN)
#define CLR_LIGHTRED (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define CLR_LIGHTGREEN (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define CLR_LIGHTBLUE (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define CLR_LIGHTCYAN (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
typedef uint16_t Colour;
#endif /* VSH_WINDOWS */

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

bool changeDirectory(const char* dir);
bool listDirectory(void);

void psInit(void);
void psUninit(void);

void setColour(Colour colour);