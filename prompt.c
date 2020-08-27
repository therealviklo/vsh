#include "prompt.h"

void printPrompt(void)
{
	if (retOn)
	{
		setColour(CLR_LIGHTCYAN);
		printf("[%i] ", exitStat);
	}

	char currUsername[1024];
	if (currUser(currUsername, sizeof(currUsername) / sizeof(*currUsername)))
	{
		setColour(CLR_LIGHTRED);
		printf("%s", currUsername);
		setColour(CLR_WOB);
		printf(":");
	}

	char currPath[1024];
	if (currDir(currPath, sizeof(currPath) / sizeof(*currPath)))
	{
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