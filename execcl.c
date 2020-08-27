#include "execcl.h"

int executeStr(const char* str)
{
	while (isspace(*str)) str++;

	if (strcmp(str, "ret on") == 0)
	{
		retOn = true;
		return 0;
	}
	else if (strcmp(str, "ret off") == 0)
	{
		retOn = false;
		return 0;
	}
	else if (strcmp(str, "ret") == 0)
	{
		printf("\x1b[32mvsh\x1b[0m: %i\n", exitStat);
		return 0;
	}
	else if (strcmp(str, "ls") == 0 || strcmp(str, "dir") == 0)
	{
#ifdef VSH_LINUX
		executeStr("ls --color=auto");
#endif /* VSH_LINUX */
		return 0;
	}

	ExecuteStatus es = execute(str);
	switch (es.error)
	{
		case ESE_INVALID:
		{
			printf("\x1b[31mvsh\x1b[0m: invalid command\n");
		}
		break;
		case ESE_MEM:
		{
			printf("\x1b[31mvsh\x1b[0m: memory error\n");
		}
		break;
		case ESE_ABNORMALEXIT:
		{
			printf("\x1b[31mvsh\x1b[0m: program exited abnormally\n");
		}
		break;
		default:
		break;
	}
	return es.ret;
}