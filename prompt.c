#include "prompt.h"

void printPrompt(void)
{
	if (retOn)
	{
		setColour(CLR_LIGHTCYAN);
		printf("[%i] ", exitStat);
	}

	bool usernamePrinted = false;
	char currUsername[1024];
	if (currUser(currUsername, sizeof(currUsername) / sizeof(*currUsername)))
	{
		setColour(CLR_LIGHTRED);
		printf("%s", currUsername);
		usernamePrinted = true;
	}

	char currPath[1024];
	if (currDir(currPath, sizeof(currPath) / sizeof(*currPath)))
	{
		if (usernamePrinted)
		{
			setColour(CLR_WOB);
			printf("|");
		}
		
		setColour(CLR_LIGHTBLUE);
		for (char* i = currPath; *i != '\0'; i++)
		{
			switch (*i)
			{
				case '/':
				case '\\':
				case ':':
				{
					setColour(CLR_WOB);
					printf("%c", *i);
					setColour(CLR_LIGHTGREEN);
				}
				break;
				default:
				{
					printf("%c", *i);
				}
				break;
			}
		}
	}
	setColour(CLR_WOB);
	printf("> ");
}