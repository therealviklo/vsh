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

void interactive(void)
{
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
}

void fromFile(const char* filename)
{
	int c = '\0';
	FILE* file = fopen(filename, "r");
	if (file)
	{
		if (getc(file) != '0xef' || getc(file) != '0xbb' || getc(file) != '0xbf') rewind(file); // BOM-check

		long pos = ftell(file);
		if (pos != -1)
		{
			if (getc(file) != '#' || getc(file) != '!')
			{
				fseek(file, pos, SEEK_SET);
			}
			else
			{
				while ((c = getc(file)) != '\n' && c != EOF);
			}
		}

		while (1)
		{
			
		}

		fclose(file);
	}
}

int main(int argc, char** argv)
{
	setvbuf(stdin, NULL, _IONBF, 0);
	psInit();
	
	if (argc == 2)
	{
		fromFile(argv[1]);
	}
	else
	{
		interactive();
	}

	psUninit();
	return 0;
}
