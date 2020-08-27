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
	else if (strcmp(str, "ls") == 0 || strcmp(str, "dir") == 0)
	{
#ifdef VSH_LINUX
		processExecuteStatus(executeStr("ls --color=auto"));
#endif /* VSH_LINUX */
		return;
	}

	processExecuteStatus(execute(str));
}