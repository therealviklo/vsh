#include "ss.h"

SS* SScreate(void)
{
	SS* ssp = malloc(sizeof(SS));
	if (ssp)
	{
		ssp->str = calloc(1, sizeof(char));
		ssp->size = 1;
		if (ssp->str)
		{
			return ssp;
		}
		free(ssp);
	}
	return NULL;
}

void SSfree(SS* ss)
{
	free(ss->str);
	free(ss);
}

void SSadd(SS* ss, char c)
{
	char* newBlock = realloc(ss->str, (ss->size + 1) * sizeof(char));
	if (newBlock)
	{
		ss->str = newBlock;
		ss->size++;
		ss->str[ss->size - 2] = c;
		ss->str[ss->size - 1] = '\0';
	}
}