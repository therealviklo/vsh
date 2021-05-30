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
		printPrompt();
		char* const str = getLine();
		setColour(CLR_WOB);
		fflush(stdout);
		if (strcmp(str, "exit") == 0)
		{
			free(str);
			break;
		}
		if (strcmp(str, "") != 0) executeStr(str);
		free(str);
	}
}

void fromFile(const char* filename)
{
	int c = '\0';
	FILE* file = fopen(filename, "r");
	if (file)
	{
		if (getc(file) != '\xef' || getc(file) != '\xbb' || getc(file) != '\xbf') rewind(file); // BOM-check

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
			SS* ss = SScreate();
			if (ss)
			{
				int c;
				while ((c = getc(file)) != '\n' && c != EOF) SSadd(ss, c);

				if (strcmp(ss->str, "exit") == 0)
				{
					c = EOF;
				}
				else if (strcmp(ss->str, "") != 0)
				{
					executeStr(ss->str);
				}
				
				SSfree(ss);
				if (c == EOF) break;
			}
			else
			{
				break;
			}
		}

		fclose(file);
	}
	else
	{
		vshMsg("file not found", MCLR_ERROR);
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
	else if (argc > 2)
	{
		if (strcmp(argv[1], "-c") == 0 && argc == 3)
		{
			executeStr(argv[2]);
		}
		else
		{
			vshMsg("invalid syntax", MCLR_ERROR);
		}
	}
	else
	{
		interactive();
	}

	psUninit();
	return 0;
}
