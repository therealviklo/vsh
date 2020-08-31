#include "ps.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool otherProcessShallRun = false;

BOOL WINAPI CtrlCHandlerRoutine(DWORD ctrlType)
{
	if (ctrlType == CTRL_C_EVENT)
	{
		otherProcessShallRun = false;
		return TRUE;
	}
	return FALSE;
}

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
			otherProcessShallRun = true;
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
				if (!otherProcessShallRun)
				{
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

bool changeDirectory(const char* dir)
{
	return SetCurrentDirectoryA(dir);
}

bool listDirectory(void)
{
	WIN32_FIND_DATAA findData;
	HANDLE handle = FindFirstFileA(".\\*", &findData);
	if (handle == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			return true;
		}
		return false;
	}
	do
	{
		if (strcmp(findData.cFileName, ".") != 0 &&
			strcmp(findData.cFileName, "..") != 0)
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				setColour(CLR_LIGHTBLUE);
			}
			else
			{
				setColour(CLR_WOB);
			}
			printf("%s\n", (const char*)findData.cFileName);
		}
	} while (FindNextFileA(handle, &findData));
	FindClose(handle);
	return true;
}

bool deleteFile(const char* rest)
{
	return DeleteFileA(rest);
}

bool createDirectory(const char* dir)
{
	return CreateDirectoryA(dir, NULL);
}

bool deleteDirectory(const char* dir)
{
	return RemoveDirectoryA(dir);
}

void psInit(void)
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCtrlHandler(CtrlCHandlerRoutine, TRUE);
}

void psUninit(void)
{
	// Inget
}

void setColour(Colour colour)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);
}

void clearScreen(void)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD charsWritten;
	CONSOLE_SCREEN_BUFFER_INFO info;
	
	if(!GetConsoleScreenBufferInfo(handle, &info)) return;

	if (!FillConsoleOutputCharacter(
		handle,
		' ',
		info.dwSize.X * info.dwSize.Y,
		(COORD){0, 0},
		&charsWritten
	)) return;

	if (!FillConsoleOutputAttribute(
		handle,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		info.dwSize.X * info.dwSize.Y,
		(COORD){0, 0},
		&charsWritten
	)) return;

	SetConsoleCursorPosition(handle, (COORD){0, 0});
}

ScreenSize getScreenSize(void)
{
	DWORD width = GetSystemMetrics(SM_CXSCREEN);
	DWORD height = GetSystemMetrics(SM_CYSCREEN);
	if (width == 0 || height == 0) return (ScreenSize){0, 0};
	return (ScreenSize){width, height};
}

CursorPos getCursorPos(void)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
	{
		return (CursorPos){info.dwCursorPosition.X, info.dwCursorPosition.Y};
	}
	return (CursorPos){0, 0};
}