#include "Windows.h"
#include <stdio.h>

void printError(const char *message)
{
	LPVOID lpvMessageBuffer;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpvMessageBuffer,
		0,
		NULL);
	fprintf(stderr, "%s: %s\n", message, lpvMessageBuffer);
	LocalFree(lpvMessageBuffer);
}

HANDLE childProcess = NULL;

int execute(LPTSTR commandLine)
{
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi = {};
	if (!CreateProcess(
		NULL,
		commandLine,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi)
	) {
		printError("CreateProcess");
		return -1;
	}
	childProcess = pi.hProcess;

	if (!CloseHandle(pi.hThread)) {
		printError("CloseHandle(hThread)");
		return -1;
	}

	printf("child processId=%d\n", pi.dwProcessId);

	DWORD r = WaitForSingleObject(childProcess, INFINITE);
	switch(r) {
	case WAIT_FAILED:
		printError("wait result=WAIT_FAILED");
		return -1;
	case WAIT_ABANDONED:
		printf("wait result=WAIT_ABANDONED\n");
		return -1;
	case WAIT_OBJECT_0:
		printf("wait result=WAIT_OBJECT_0\n");
		break;
	case WAIT_TIMEOUT:
		printf("wait result=WAIT_TIMEOUT\n");
		return -1;
	default:
		printf("wait result=%d\n", r);
		return -1;
	}

	DWORD exitCode;
	if (!GetExitCodeProcess(childProcess, &exitCode)) {
		printError("GetExitCodeProcess");
		return -1;
	}
	printf("exitCode=%d/%x\n", exitCode, exitCode);

	return exitCode;
}