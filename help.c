#include "help.h"

const char* const helpTable[] = {
	"cd DIRECTORY", "Changes directory.",
	"clear", "Clears the screen.\n"
			 "(Same as \"cls\")",
	"cls", "Clears the screen.\n"
		   "(Same as \"clear\")",
	"del FILE", "Removes a file.\n"
				"(Same as \"rm\")",
	"dir", "Windows: Prints a list of the files in the current directory.\n"
	       "Linux: Runs ls with colours turned on.\n"
		   "(Same as \"ls\")",
	"exit", "Exits.",
	"help", "Shows this message.",
	"ls", "Windows: Prints a list of the files in the current directory.\n"
	      "Linux: Runs ls with colours turned on.\n"
		  "(Same as \"dir\")",
	"mk FILE", "Creates a file.",
	"mkdir DIRECTORY", "Creates a directory.",
	"path", "Prints the path of the current directory.",
	"path off", "Removes the path from the prompt.",
	"path on", "Adds the path to the prompt",
	"ret", "Prints the last return value.",
	"ret off", "Removes the return value from the prompt.",
	"ret on", "Adds the return value to the prompt.",
	"rm FILE", "Removes a file.\n"
			   "(Same as \"del\")",
	"rmdir DIRECTORY", "Removes a directory.",
	"system COMMAND", "Sends the rest of the command to C's system() function.",
	"user", "Prints the name of the current user.",
	"user off", "Removes the username from the prompt.",
	"user on", "Adds the username to the prompt."
};

void printUTF8Character(const char** currPos, size_t* charsWritten)
{
	if (0b10000000 & **currPos)
	{
		do
		{
			putchar(**currPos);
			(*currPos)++;
		} while ((0b10000000 & **currPos) && !(0b01000000 & **currPos));
	}
	else
	{
		putchar(**currPos);
		(*currPos)++;
	}
	(*charsWritten)++;
}

void switchHelpTableLine(size_t* charsWritten, size_t width, size_t longestCommandStrLen)
{
	putchar('\n');
	*charsWritten += width - (*charsWritten % width);
	
	if (longestCommandStrLen < width) // <-- Inga oändliga loopar.
	{
		while ((*charsWritten % width) < longestCommandStrLen)
		{
			putchar(' ');
			(*charsWritten)++;
		}
	}
}

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
			printUTF8Character(&currPos, &charsWritten);
		}
		while (charsWritten < longestCommandStrLen)
		{
			putchar(' ');
			charsWritten++;
		}
		
		currPos = helpTable[2 * i + 1]; // Återanvänder
		while (*currPos != '\0')
		{
			while (isspace(*currPos))
			{
				bool switchedLine = false;
				if (charsWritten % screenSize.width + 2 >= screenSize.width)
				{
					switchHelpTableLine(&charsWritten, screenSize.width, longestCommandStrLen);
					switchedLine = true;
				}
				if (charsWritten % screenSize.width > longestCommandStrLen)
				{
					if (*currPos == '\n')
					{
						if (!switchedLine)
							switchHelpTableLine(&charsWritten, screenSize.width, longestCommandStrLen);
					}
					else
					{
						putchar(*currPos);
						charsWritten++;
					}
				}
				currPos++;
			}

			const char* currWordPos = currPos;
			size_t wordLen = 0;
			while (!isspace(*currWordPos) && *currWordPos != '\0')
			{
				wordLen++;
				currWordPos++;
			}

			if ((charsWritten % screenSize.width) + wordLen >= screenSize.width)
			{
				switchHelpTableLine(&charsWritten, screenSize.width, longestCommandStrLen);
			}

			while (!isspace(*currPos) && *currPos != '\0')
			{
				printUTF8Character(&currPos, &charsWritten);
			}
			// printUTF8Character(&currPos, &charsWritten);
			// if (charsWritten % (screenSize.width - 2) == 0)
			// {
			// 	if (*currPos == ' ') currPos++;
			// 	else if (*(currPos - 1) != ' ') putchar('-');
			// 	putchar('\n');
			// 	for (size_t j = 0; j < longestCommandStrLen; j++)
			// 	{
			// 		putchar(' ');
			// 		charsWritten++;
			// 	}
			// }
		}
		putchar('\n');
		if (i < sizeof(helpTable) / sizeof(*helpTable) / 2 - 1) putchar('\n');
	}

	fflush(stdout);
}