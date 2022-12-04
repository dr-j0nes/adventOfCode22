#include <Windows.h>
#include <stdio.h>

//
// need to be static for REASONS
// 0x1000 = 4096 = 1 page (ok not always but always)
//
#define BUFFERSIZE 0x1000
#define INPUT_FILE L"../Input/day1pt1.txt"

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
	LPDWORD lpBytesRead = &dwBytesRead;
	LPVOID lpBuffer = malloc(BUFFERSIZE); // CAN FAIL CHECK THIS
	DWORD dwCurrBuffSize = 0;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwMaxValue = 0;

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
		INPUT_FILE,
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

	do {
		bCheck = ReadFile(
			hFile,
			lpBuffer,
			BUFFERSIZE - 1,
			lpBytesRead,
			NULL
		);

		if (bCheck == FALSE)
		{
			errorPrinter(L"ReadFile errror!");
			retVal = 3300;
			goto exit;
		}

		//
		// check for EOF
		// there's probably a more elegant way to do this, but breaking works
		//
		if (bCheck && (dwBytesRead == 0))
		{
			break;
		}

		DWORD i = 0;
		//i = atoi((char*)(lpBuffer)+ sizeof(char)*5);
		//printf("%d\n", i);
		//break;

		char holder[50] = { 0 };
		DWORD dwCount = 0;
		DWORD dwTest = 0;
		char cLastChar = 'a';
		BOOL bCarRet = FALSE;

		for (i = 0; i < BUFFERSIZE - 1; i++)
		{
			char x;
			x = ((char*)(lpBuffer))[i];
			if (x == '\n')
			{
				if (bCarRet == TRUE) 
				{
					printf("\tcarriage return\n");
				}
				else
				{
					dwTest = atoi((char*)(lpBuffer)+sizeof(char) * i);
					printf("found int\t%d\n", dwTest);
					bCarRet = TRUE;
				}
			}
			if (x == '\r')
			{

			}
			else {
				bCarRet = FALSE;
			}

		}

		dwCurrBuffSize += dwBytesRead;
		printf("Bytes Read:\t%d\n", dwCurrBuffSize);
		RtlZeroMemory(lpBuffer, BUFFERSIZE);

	} while (TRUE); 

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