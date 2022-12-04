#include <Windows.h>
#include <stdio.h>

//
// need to be static for REASONS
// 0x1000 = 4096 = 1 page (ok not always but always)
//
#define BUFFERSIZE 0x1000

//
// ngl kinda don't like this name
//
void errorPrinter(LPTSTR printer)
{
	DWORD dwError = 0;
	dwError = GetLastError();
	printf("%ls\n", printer);
	printf("ERROR\t%d\n", dwError);
}

int main() {
	DWORD retVal = 1111;
	BOOL bCheck = FALSE;
	DWORD dwBytesRead = 0;
	LPDWORD pBytesRead = &dwBytesRead;
	LPVOID lpBuffer = malloc(BUFFERSIZE); // CAN FAIL CHECK THIS
	DWORD dwCurrBuffSize = 0;
	HANDLE hFile = INVALID_HANDLE_VALUE;

	if (lpBuffer == NULL)
	{
		errorPrinter(L"malloc error everything is fucked!");
		retVal = 2222;
		goto exit;
	}
	RtlSecureZeroMemory(lpBuffer, BUFFERSIZE); // not strictly necessary but whatever

	//
	// open read....
	//
	hFile = CreateFile(
		L"../XXXInput/day1pt1.txt",
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		errorPrinter(L"file open error!");
		retVal = 3333;
		goto exit;
	}

	//
	// the one we hit when things are good
	//
	retVal = 0;
	goto exit;

exit:

	if (INVALID_HANDLE_VALUE != hFile)
	{
		CloseHandle(hFile);
	}
	return retVal;
}