#include <stdio.h>
#include "ss.h"
#include "ps.h"
#include "prompt.h"
#include "execcl.h"
#include "ret.h"

#include "ss.c"
#include "ps.c"
#include "cl.c"
#include "prompt.c"
#include "execcl.c"
#include "ret.c"
#include "utils.c"
#include "vshmsg.c"
#include "help.c"

int main()
{
	setvbuf(stdin, NULL, _IONBF, 0);
	psInit();
	while (1)
	{
		SS* ss = SScreate();
		if (ss)
		{
			printPrompt();
			int c = '\0';
			while ((c = getchar()) != '\n' && c != EOF) SSadd(ss, c);
			setColour(CLR_WOB);
			fflush(stdout);
			if (strcmp(ss->str, "exit") == 0)
			{
				SSfree(ss);
				break;
			}
			executeStr(ss->str);
			SSfree(ss);
		}
		else
		{
			break;
		}
	}
	psUninit();
	return 0;
}
