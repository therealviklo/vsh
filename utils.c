#include "utils.h"

const char* startsWith(const char* str, const char* prefix)
{
	while (1)
	{
		if (*prefix == '\0') return str;
		if (*prefix != *str) break;
		str++;
		prefix++;
	}
	return NULL;
}