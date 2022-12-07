#include "Shared.h"

//
// need to be static for REASONS
// 0x1000 = 4096 = 1 page (ok not always but always)
//
#define BUFFERSIZE 0x1000
#define INPUT_FILE L"../Input/day1pt1.txt"


int day1() {
	DWORD retVal = 1111;
	BOOL bCheck = FALSE;
	DWORD dwBytesRead = 0;
	LPDWORD lpBytesRead = &dwBytesRead;
	LPVOID lpBuffer = malloc(BUFFERSIZE); // CAN FAIL CHECK THIS
	DWORD dwCurrBuffSize = 0;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwBiggest = 0;

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

	//
	// honestly should encapsulate the file read
	//

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
		DWORD dwCount = 0;
		DWORD dwTest = 0;
		BOOL bSkip = FALSE;
		DWORD collection[50] = { 0 };
		for (i = 0; i < BUFFERSIZE - 1; i++)
		{
			// skip new lines - atoi will consume them and double count the first
			//element of each block
			if (bSkip)
			{
				bSkip = FALSE;
				continue;
			}
			char x;
			x = ((char*)(lpBuffer))[i];
			if (x == '\n')
			{
				dwTest = atoi((char*)(lpBuffer)+sizeof(char) * i);
				printf("found int\t%d\n", dwTest);
				collection[dwCount] = dwTest;
				dwCount += 1;
			}
			if (x == '\r')
			{
				//
				// prevent us reading past the end of the buffer
				// should never happen but *shrug*
				//
				if ((i + 2) > BUFFERSIZE - 1)
				{
					printf("hit break\n");
					break;
				}
				//
				// if '\r' ahead of us, means we're at THE END of a block
				//
				if (((char*)(lpBuffer))[i + 2] == '\r')
				{
					printf("\tdoing our sums!\n");
					DWORD sum = 0;
					for (DWORD j = 0; j < dwCount; j++)
					{
						sum += collection[j];
					}
					printf("\tsum:\t %d\n", sum);
					if (sum > dwBiggest)
					{
						printf("\tNEW BIGGEST\n");
						dwBiggest = sum;
					}

					// clean up
					dwCount = 0;
					memset(collection, 0, sizeof(collection));
					bSkip = TRUE;
				}
			}

		}
		RtlZeroMemory(lpBuffer, BUFFERSIZE);

	} while (TRUE);


	printf("biggest group: %d", dwBiggest);
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

	if (NULL != lpBuffer)
	{
		free(lpBuffer);
	}

	return retVal;
}