#include "execcl.h"

void processExecuteStatus(ExecuteStatus es)
{
	exitStat = es.ret;
	switch (es.error)
	{
		case ESE_INVALID:
		{
			vshMsg("invalid command", MCLR_ERROR);
		}
		break;
		case ESE_MEM:
		{
			vshMsg("memory error", MCLR_ERROR);
		}
		break;
		case ESE_ABNORMALEXIT:
		{
			vshMsg("program exited abnormally", MCLR_ERROR);
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
		vshMsg(exitStat, MCLR_NOTICE);
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
			vshMsg(currUsername, MCLR_NOTICE);
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
			vshMsg(currPath, MCLR_NOTICE);
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
			vshMsg("unable to enter directory", MCLR_ERROR);
			exitStat = 0;
			return;
		}
	}
	else if (strcmp(str, "ls") == 0 || strcmp(str, "dir") == 0)
	{
		if (!listDirectory())
		{
			vshMsg("unable to list directory", MCLR_ERROR);
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
			vshMsg("unable to delete file", MCLR_ERROR);
		}
		exitStat = 0;
		return;
	}
	else if ((start = startsWith(str, "mk ")))
	{
		FILE* file = fopen(start, "ab");
		if (file)
		{
			fclose(file);
		}
		else
		{
			vshMsg("unable to create file", MCLR_ERROR);
		}
		exitStat = 0;
		return;
	}
#ifdef VSH_WINDOWS
	else if ((start = startsWith(str, "rmdir ")))
	{
		if (!deleteDirectory(start))
		{
			vshMsg("unable to delete directory", MCLR_ERROR);
		}
		exitStat = 0;
		return;
	}
	else if ((start = startsWith(str, "mkdir ")))
	{
		if (!createDirectory(start))
		{
			vshMsg("unable to create directory", MCLR_ERROR);
		}
		exitStat = 0;
		return;
	}
#endif /* VSH_WINDOWS */
	else if (strcmp(str, "cls") == 0 || strcmp(str, "clear") == 0)
	{
		clearScreen();
		exitStat = 0;
		return;
	}
	else if ((start = startsWith(str, "system ")))
	{
		if (system(NULL))
		{
			system(start);
		}
		else
		{
			vshMsg("no command processor is available", MCLR_ERROR);
		}
		exitStat = 0;
		return;
	}

	processExecuteStatus(execute(str));
}