#include "ProcessBuilder.h"

int main(int argc)
{
	printf("NGROK[%d] start\n", GetCurrentProcessId());

	int r = execute("ngrok.exe tcp -region jp 25565");

	if (childProcess != NULL) {
		if(!CloseHandle(childProcess)) {
			printError("CloseHandle(childProcess)");
		}
	}

	printf("NGROK[%d] end\n", GetCurrentProcessId());
    system("pause");
	return r;
}