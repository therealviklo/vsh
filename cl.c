#include "cl.h"

char* CLcreate_extractParam(const char** str)
{
	char* buf = calloc(1, sizeof(char));
	if (buf)
	{
		size_t size = 1;
		char c = '\0';
		
		enum {
			QM_NOQUOTE,
			QM_DOUBLEQUOTE,
			QM_SINGLEQUOTE
		} quoteMode = QM_NOQUOTE;
		if (**str == '"')
		{
			quoteMode = QM_DOUBLEQUOTE;
			(*str)++;
		}
		else if (**str == '\'')
		{
			quoteMode = QM_SINGLEQUOTE;
			(*str)++;
		}

		while (1)
		{
			c = *((*str)++); // Sätt c till det tecken som *str pekar till och gör så att *str pekar till nästa tecken.
			if (c == '\0')
			{
				if (quoteMode)
				{
					free(buf);
					return NULL;
				}
				(*str)--;
				break;
			}
			else if ((quoteMode == QM_SINGLEQUOTE && c == '\'') || (quoteMode == QM_DOUBLEQUOTE && c == '"') || (isspace(c) && !quoteMode))
			{
				break;
			}
			char* newBuf = realloc(buf, (size + 1) * sizeof(char));
			if (!newBuf)
			{
				free(buf);
				return NULL;
			}
			buf = newBuf;
			size++;
			buf[size - 2] = c;
		}
		while (isspace(**str)) (*str)++;
		buf[size - 1] = '\0';
		return buf;
	}
	return NULL;
}

char** CLcreate_extractParams(char* path, const char** str)
{
	char** argv = calloc(2, sizeof(char*));
	if (argv)
	{
		size_t size = 2;
		argv[0] = malloc(sizeof(char) * (strlen(path) + 1));
		if (argv[0])
		{
			memcpy(argv[0], path, strlen(path) + 1);
			while (**str != '\0')
			{
				char* arg = CLcreate_extractParam(str);
				if (arg)
				{
					char** newArgv = realloc(argv, (size + 1) * sizeof(char*));
					if (newArgv)
					{
						size++;
						argv = newArgv;
						argv[size - 2] = arg;
						continue;
					}
					free(arg);
				}
				for (size_t i = 0; i < size - 1; i++)
				{
					free(argv[i]);
				}
				free(argv);
				return NULL;
			}
			argv[size - 1] = NULL;
			return argv;
		}
		free(argv);
		return NULL;
	}
	return NULL;
}

CL* CLcreate(const char* str)
{
	CL* cl = malloc(sizeof(CL));
	if (cl)
	{
		cl->path = CLcreate_extractParam(&str);
		if (cl->path)
		{
			cl->argv = CLcreate_extractParams(cl->path, &str);
			if (cl->argv)
			{
				return cl;
			}
			free(cl->path);
		}
		free(cl);
	}
	return NULL;
}

void CLfree(CL* cl)
{
	free(cl->path);
	for (char** i = cl->argv; *i != NULL; i++)
	{
		free(*i);
	}
	free(cl->argv);
}