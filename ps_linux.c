#include "ps.h"
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <termios.h>
#include "cl.h"
#include "vshmsg.h"
#include "ss.h"

ExecuteStatus execute_cl(CL* cl)
{
	pid_t pid;
	static const struct timespec ts = {0, 1};
	if ((pid = fork()) == 0)
	{
		signal(SIGINT, SIG_DFL);
		execvp(cl->path, cl->argv);
		exit(127);
	}

	int status;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 127)
			return (ExecuteStatus){0, ESE_INVALID};
		else
			return (ExecuteStatus){WEXITSTATUS(status), ESE_SUCCESS};
	}
	return (ExecuteStatus){0, ESE_ABNORMALEXIT};
}

ExecuteStatus execute(const char* str)
{
	CL* cl = CLcreate(str);
	if (cl)
	{
		ExecuteStatus r = execute_cl(cl);
		CLfree(cl);
		if (r.error == ESE_INVALID)
		{
			size_t bufferSize = strlen(str) + 3;
			char* buffer = malloc(sizeof(char) * bufferSize);
			if (buffer)
			{
				if (*str == '"')
				{
					int ret = snprintf(buffer, sizeof(char) * bufferSize, "\"./%s", &str[1]);
					if (ret < 0 || ret >= sizeof(char) * bufferSize)
					{
						free(buffer);
						return (ExecuteStatus){0, ESE_MEM};
					}
				}
				else
				{
					int ret = snprintf(buffer, sizeof(char) * bufferSize, "./%s", str);
					if (ret < 0 || ret >= sizeof(char) * bufferSize)
					{
						free(buffer);
						return (ExecuteStatus){0, ESE_MEM};
					}
				}
				ExecuteStatus r2 = {0, ESE_MEM};
				CL* cl2 = CLcreate(buffer);
				if (cl2)
				{
					r2 = execute_cl(cl2);
					CLfree(cl2);
				}
				free(buffer);
				return r2;
			}
			return (ExecuteStatus){0, ESE_MEM};
		}
		return r;
	}
	return (ExecuteStatus){0, ESE_MEM};
}

bool currDir(char* buffer, size_t size)
{
	if (getcwd(buffer, size)) return true;
	return false;
}

bool currUser(char* buffer, size_t size)
{
	struct passwd* pwd = getpwuid(getuid());
	if (pwd)
	{
		size_t usernameBufLen = strlen(pwd->pw_name) + 1;
		if (size >= usernameBufLen)
		{
			memcpy(buffer, pwd->pw_name, usernameBufLen);
			return true;
		}
	}
	return false;
}

bool changeDirectory(const char* dir)
{
	return chdir(dir) != -1;
}

bool changeDirectoryHome(void)
{
	const char* const home = getenv("HOME");
	if (home) return changeDirectory(home);
	return false;
}

bool listDirectory(void)
{
	return execute("ls --color=auto").error == ESE_SUCCESS;
}

bool deleteFile(const char* rest)
{
	size_t bufLen = sizeof("rm ") / sizeof(char) + strlen(rest) + 1;
	char* buffer = malloc(sizeof(char) * bufLen);
	if (buffer)
	{
		int r = snprintf(buffer, bufLen, "rm %s", rest);
		if (r >= 0 && r < bufLen)
		{
			ExecuteStatus es = execute(buffer);
			free(buffer);
			return es.error == ESE_SUCCESS;
		}

		vshMsg("memory error", MCLR_ERROR);
		free(buffer);
		return false;
	}
	else
	{
		vshMsg("memory error", MCLR_ERROR);
		return false;
	}
	return true;
}

void psInit(void)
{
	signal(SIGINT, SIG_IGN);
}

void psUninit(void)
{
	// Inget
}

void setColour(Colour colour)
{
	printf("\x1b[%im", (int)colour);
}

void clearScreen(void)
{
	execute("clear");
}

ScreenSize getScreenSize(void)
{
	struct winsize w;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1)
		return (ScreenSize){w.ws_col, w.ws_row};
	return (ScreenSize){0, 0};
}

size_t getCursorPos_stringToSize(const char* str)
{
	size_t sum = 0;
	while (*str != '\0')
	{
		sum *= 10;
		sum += *str - '0';
		str++;
	}
	return sum;
}

void getCursorPos_noEcho(bool enable)
{
	static struct termios old;
	static struct termios new;

	if (enable)
	{
		tcgetattr(0, &old);
		new = old;
		new.c_lflag &= ~ICANON;
		new.c_lflag &= ~ECHO;
		tcsetattr(0, TCSANOW, &new);
	}
	else
	{
		tcsetattr(0, TCSANOW, &old);
	}
}

CursorPos getCursorPos(void)
{
	getCursorPos_noEcho(true);

	printf("\x1b[6n");
	fflush(stdout);

	while (getchar() != '\x1b');
	if (getchar() != '[')
	{
		getCursorPos_noEcho(false);
		return (CursorPos){0, 0};
	}

	SS* yss = SScreate();
	if (yss)
	{
		int c;
		while ((c = getchar()) != ';' && c != EOF) SSadd(yss, c);
		if (c != EOF)
		{
			SS* xss = SScreate();
			if (xss)
			{
				while ((c = getchar()) != 'R' && c != EOF) SSadd(xss, c);
				if (c != EOF)
				{
					size_t x = getCursorPos_stringToSize(xss->str);
					size_t y = getCursorPos_stringToSize(yss->str);
					SSfree(yss);
					SSfree(xss);
					getCursorPos_noEcho(false);
					return (CursorPos){x, y};
				}
				SSfree(xss);
			}
		}
		SSfree(yss);
	}
	getCursorPos_noEcho(false);
	return (CursorPos){0, 0};
}