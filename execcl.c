#include "execcl.h"

char* extractSingleParam(const char* str)
{
	if (*str == '\'')
	{
		str++;
		const size_t bufLen = strlen(str) + 1;
		char* buf = malloc(bufLen * sizeof(char));
		if (buf)
		{
			size_t i = 0;
			for (; str[i] != '\''; i++)
			{
				if (str[i] == '\0')
				{
					vshMsg("syntax error", MCLR_ERROR);
					free(buf);
					return NULL;
				}
				buf[i] = str[i];
			}
			buf[i] = '\0';
			i++;

			// Kontrollera att det inte finns saker efter.
			while (isspace(str[i])) i++;
			if (str[i] != '\0')
			{
				vshMsg("syntax error", MCLR_ERROR);
				free(buf);
				return NULL;
			}

			return buf;
		}
	}
	else if (*str == '\"')
	{
		str++;
		const size_t bufLen = strlen(str) + 1;
		char* buf = malloc(bufLen * sizeof(char));
		if (buf)
		{
			size_t i = 0;
			for (; str[i] != '"'; i++)
			{
				if (str[i] == '\0')
				{
					vshMsg("syntax error", MCLR_ERROR);
					free(buf);
					return NULL;
				}
				buf[i] = str[i];
			}
			buf[i] = '\0';
			i++;

			// Kontrollera att det inte finns saker efter.
			while (isspace(str[i])) i++;
			if (str[i] != '\0')
			{
				vshMsg("syntax error", MCLR_ERROR);
				free(buf);
				return NULL;
			}

			return buf;
		}
	}
	else
	{
		const size_t bufLen = strlen(str) + 1;
		char* buf = malloc(bufLen * sizeof(char));
		if (buf)
		{
			memcpy(buf, str, bufLen * sizeof(char));
			return buf;
		}
	}
	vshMsg("memory error", MCLR_ERROR);
	return NULL;
}

void skipWhitespace(const char** strptr)
{
	while (isspace(**strptr)) (*strptr)++;
}

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
		size_t bufLen = 1024;
		char* buffer = malloc(bufLen * sizeof(char));
		if (buffer)
		{
			int ret = snprintf(buffer, bufLen, "%i", exitStat);
			if (ret >= 0 && ret < bufLen)
			{
				vshMsg(buffer, MCLR_NOTICE);
			}
			else
			{
				vshMsg("snprintf error", MCLR_ERROR);
			}
			free(buffer);
		}
		else
		{
			vshMsg("memory error", MCLR_ERROR);
		}
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
		skipWhitespace(&start);

		char* paramBuf = extractSingleParam(start);
		if (paramBuf)
		{
			if (!changeDirectory(paramBuf))
			{
				vshMsg("unable to enter directory", MCLR_ERROR);
			}
			free(paramBuf);
		}
		exitStat = 0;
		return;
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
		skipWhitespace(&start);
		
		char* paramBuf = extractSingleParam(start);
		if (paramBuf)
		{
			size_t bufferLen = (strlen(paramBuf) + sizeof(" --color=auto") / sizeof(char) + 1);
			char* buffer = malloc(sizeof(char) * bufferLen);
			if (buffer)
			{
				int r = snprintf(buffer, bufferLen, "ls --color=auto %s", paramBuf);
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

			free(paramBuf);
		}
		exitStat = 0;
		return;
	}
#endif /* VSH_LINUX */
	else if ((start = startsWith(str, "rm ")) || (start = startsWith(str, "del ")))
	{
		skipWhitespace(&start);
		
		char* paramBuf = extractSingleParam(start);
		if (paramBuf)
		{
			if (!deleteFile(paramBuf))
			{
				vshMsg("unable to delete file", MCLR_ERROR);
			}

			free(paramBuf);
		}
		exitStat = 0;
		return;
	}
	else if ((start = startsWith(str, "mk ")))
	{
		skipWhitespace(&start);
		
		char* paramBuf = extractSingleParam(start);
		if (paramBuf)
		{
			FILE* file = fopen(paramBuf, "ab");
			if (file)
			{
				fclose(file);
			}
			else
			{
				vshMsg("unable to create file", MCLR_ERROR);
			}

			free(paramBuf);
		}
		exitStat = 0;
		return;
	}
#ifdef VSH_WINDOWS
	else if ((start = startsWith(str, "rmdir ")))
	{
		skipWhitespace(&start);
		
		char* paramBuf = extractSingleParam(start);
		if (paramBuf)
		{
			if (!deleteDirectory(paramBuf))
			{
				vshMsg("unable to delete directory", MCLR_ERROR);
			}

			free(paramBuf);
		}
		exitStat = 0;
		return;
	}
	else if ((start = startsWith(str, "mkdir ")))
	{
		skipWhitespace(&start);
		
		char* paramBuf = extractSingleParam(start);
		if (paramBuf)
		{
			if (!createDirectory(paramBuf))
			{
				vshMsg("unable to create directory", MCLR_ERROR);
			}

			free(paramBuf);
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
		skipWhitespace(&start);
		
		char* paramBuf = extractSingleParam(start);
		if (paramBuf)
		{
			if (system(NULL))
			{
				system(paramBuf);
			}
			else
			{
				vshMsg("no command processor is available", MCLR_ERROR);
			}

			free(paramBuf);
		}
		exitStat = 0;
		return;
	}

	processExecuteStatus(execute(str));
}