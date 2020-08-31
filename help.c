﻿#include "help.h"

const char* const helpTable[] = {
	"cd DIRECTORY", "Change directory.",
	"help", "Show this message.",
	"system COMMAND", "Send the rest of the command to C's system() function."
};

void printHelpTable(void)
{
	ScreenSize screenSize = getScreenSize();
	if (screenSize.width <= 1 || screenSize.height == 0) return;

	int longestCommandStrLen = 0;
	for (size_t i = 0; i < sizeof(helpTable) / sizeof(*helpTable); i += 2)
	{
		const size_t len = strlen(helpTable[i]);
		if (len > longestCommandStrLen) longestCommandStrLen = len;
	}
	longestCommandStrLen += 2; // Lite mellanrum mellan kommandot och förklaringen.
	
	/* 2 * i = kommandot
	   2 * i + 1 = förklaringen */
	for (size_t i = 0; i < sizeof(helpTable) / sizeof(*helpTable) / 2; i++)
	{
		const char* currPos = helpTable[2 * i];
		size_t charsWritten = 0;
		while (*currPos != '\0')
		{
			putchar(*currPos);
			charsWritten++;
			currPos++;
		}
		while (charsWritten < longestCommandStrLen)
		{
			putchar(' ');
			charsWritten++;
		}
		
		currPos = helpTable[2 * i + 1]; // Återanvänder
		while (*currPos != '\0')
		{
			putchar(*currPos);
			charsWritten++;
			currPos++;
			if (charsWritten % (screenSize.width - 2) == 0)
			{
				if (*currPos == ' ') currPos++;
				else if (*(currPos - 1) != ' ') putchar('-');
				putchar('\n');
				for (size_t j = 0; j < longestCommandStrLen; j++)
				{
					putchar(' ');
					charsWritten++;
				}
			}
		}
		putchar('\n');
	}

	fflush(stdout);
}