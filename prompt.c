#include "prompt.h"

void printPrompt(void)
{
	if (retOn)
	{
		printf("\x1b[96m[%i] ", exitStat);
	}

	char currUsername[1024];
	if (currUser(currUsername, sizeof(currUsername) / sizeof(*currUsername)))
	{
		printf("\x1b[91m%s\x1b[97m:", currUsername);
	}

	char currPath[1024];
	if (currDir(currPath, sizeof(currPath) / sizeof(*currPath)))
	{
		printf("\x1b[92m");
		for (char* i = currPath; *i != '\0'; i++)
		{
			switch (*i)
			{
				case '/':
				case '\\':
				case ':':
				{
					printf("\x1b[97m%c\x1b[92m", *i);
				}
				break;
				default:
				{
					if (i == currPath)
					{
						printf("\x1b[94m%c\x1b[92m", *i);
					}
					else
					{
						printf("%c", *i);
					}
				}
				break;
			}
		}
	}
	printf("\x1b[97m>\x1b[0m ");
}