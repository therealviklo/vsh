#include "execcl.h"

void processExecuteStatus(ExecuteStatus es)
{
	exitStat = es.ret;
	switch (es.error)
	{
		case ESE_INVALID:
		{
			setColour(CLR_DARKRED);
			printf("vsh");
			setColour(CLR_WOB);
			printf(": invalid command\n");
		}
		break;
		case ESE_MEM:
		{
			setColour(CLR_DARKRED);
			printf("vsh");
			setColour(CLR_WOB);
			printf(": memory error\n");
		}
		break;
		case ESE_ABNORMALEXIT:
		{
			setColour(CLR_DARKRED);
			printf("vsh");
			setColour(CLR_WOB);
			printf(": program exited abnormally\n");
		}
		break;
		default:
		break;
	}
}

void executeStr(const char* str)
{
	while (isspace(*str)) str++;

	const char* start;
	if (strcmp(str, "ret on") == 0)
	{
		retOn = true;
		exitStat = 0;
		return;
	}
	else if (strcmp(str, "ret off") == 0)
	{
		retOn = false;
		exitStat = 0;
		return;
	}
	else if (strcmp(str, "ret") == 0)
	{
		setColour(CLR_DARKGREEN);
		printf("vsh");
		setColour(CLR_WOB);
		printf(": %i\n", exitStat);
		exitStat = 0;
		return;
	}
	else if (strcmp(str, "user on") == 0)
	{
		userOn = true;
		exitStat = 0;
		return;
	}
	else if (strcmp(str, "user off") == 0)
	{
		userOn = false;
		exitStat = 0;
		return;
	}
	else if (strcmp(str, "user") == 0)
	{
		char currUsername[1024];
		if (currUser(currUsername, sizeof(currUsername) / sizeof(*currUsername)))
		{
			setColour(CLR_DARKGREEN);
			printf("vsh");
			setColour(CLR_WOB);
			printf(": %s\n", currUsername);
		}
		exitStat = 0;
		return;
	}
	else if (strcmp(str, "path on") == 0)
	{
		pathOn = true;
		exitStat = 0;
		return;
	}
	else if (strcmp(str, "path off") == 0)
	{
		pathOn = false;
		exitStat = 0;
		return;
	}
	else if (strcmp(str, "path") == 0)
	{
		char currPath[1024];
		if (currDir(currPath, sizeof(currPath) / sizeof(*currPath)))
		{
			setColour(CLR_DARKGREEN);
			printf("vsh");
			setColour(CLR_WOB);
			printf(": %s\n", currPath);
		}
		exitStat = 0;
		return;
	}
	else if ((start = startsWith(str, "cd ")))
	{
		if (changeDirectory(start))
		{
			exitStat = 0;
			return;
		}
		else
		{
			setColour(CLR_DARKRED);
			printf("vsh");
			setColour(CLR_WOB);
			printf(": unable to enter directory\n");
			exitStat = 0;
			return;
		}
	}
	else if (strcmp(str, "ls") == 0 || strcmp(str, "dir") == 0)
	{
		if (!listDirectory())
		{
			setColour(CLR_DARKRED);
			printf("vsh");
			setColour(CLR_WOB);
			printf(": unable to list directory\n");
		}
		exitStat = 0;
		return;
	}
#ifdef VSH_LINUX
	else if ((start = startsWith(str, "ls ")) || (start = startsWith(str, "dir ")))
	{
		size_t bufferLen = (strlen(str) + sizeof(" --color=auto") / sizeof(char) + 1);
		char* buffer = malloc(sizeof(char) * bufferLen);
		if (buffer)
		{
			int r = snprintf(buffer, bufferLen, "ls --color=auto %s", start);
			if (r >= 0 && r < bufferLen)
			{
				processExecuteStatus(execute(buffer));
			}
			else
			{
				processExecuteStatus((ExecuteStatus){0, ESE_MEM});
			}

			free(buffer);
		}
		else
		{
			processExecuteStatus((ExecuteStatus){0, ESE_MEM});
		}
		exitStat = 0;
		return;
	}
#endif /* VSH_LINUX */
	else if ((start = startsWith(str, "rm ")) || (start = startsWith(str, "del ")))
	{
		if (!deleteFile(start))
		{
			setColour(CLR_DARKRED);
			printf("vsh");
			setColour(CLR_WOB);
			printf(": unable to delete file\n");
		}
		exitStat = 0;
		return;
	}

	processExecuteStatus(execute(str));
}