#include "ps.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>

ExecuteStatus execute(const char* str)
{
	size_t bufferSize = sizeof(char) * (strlen(str) + 1);
	char* buffer = malloc(bufferSize);
	if (buffer)
	{
		buffer[0] = '\0';
		memcpy(buffer, str, bufferSize);
		STARTUPINFOA sui = {};
		sui.cb = sizeof(sui);
		PROCESS_INFORMATION pi = {};
		if (CreateProcessA(
			NULL,
			buffer,
			NULL,
			NULL,
			FALSE,
			NORMAL_PRIORITY_CLASS,
			NULL,
			NULL,
			&sui,
			&pi
		))
		{
			DWORD exitCode = STILL_ACTIVE;
			ExecuteStatus r = {0, ESE_INVALID};
			while (1)
			{
				if (!GetExitCodeProcess(pi.hProcess, &exitCode)) break;
				if (exitCode != STILL_ACTIVE)
				{
					r.ret = exitCode;
					r.error = ESE_SUCCESS;
					break;
				}
				Sleep(1);
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			free(buffer);
			return r;
		}
		else
		{
			free(buffer);
			return (ExecuteStatus){0, ESE_INVALID};
		}
	}
	return (ExecuteStatus){0, ESE_MEM};
}

bool currDir(char* buffer, size_t size)
{
	DWORD written = GetCurrentDirectory(size, buffer);
	if (written != 0 && written <= size) return true;
	return false;
}

bool currUser(char* buffer, size_t size)
{
	DWORD s = (DWORD)size;
	return GetUserNameA(buffer, &s);
}

void psInit(void)
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
}

void psUninit(void)
{
	// Inget
}

void setColour(Colour colour)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);
}