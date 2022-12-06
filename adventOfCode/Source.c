#include <Windows.h>
#include <stdio.h>

//
// need to be static for REASONS
// 0x1000 = 4096 = 1 page (ok not always but always)
//
#define BUFFERSIZE 0x1000
#define INPUT_FILE L"../Input/day1pt1.txt"

typedef struct
{
	DWORD dwFirst;
	DWORD dwSecond;
	DWORD dwThird;
} TopThree;

//
// insert into our TopThree struct, shifting as needed
//
void insert(TopThree* top, DWORD pos, DWORD dwNewVal)
{
	DWORD dwSave = 0;

	if (pos == 1)
	{
		dwSave = top->dwFirst;
		top->dwFirst = dwNewVal;
		top->dwThird = top->dwSecond;
		top->dwSecond = dwSave;
	}

	if (pos == 2)
	{
		top->dwThird = top->dwSecond;
		top->dwSecond = dwNewVal;
	}

}


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
	TopThree top = { 0 };
	TopThree* lpTop = &top;

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
					if (sum > top.dwFirst)
					{
						insert(lpTop, 1, sum);
						goto cleanup;
					}

					if (sum > top.dwSecond)
					{
						insert(lpTop, 2, sum);
						goto cleanup;
					}
					if (sum > top.dwThird)
					{
						top.dwThird = sum;
					}

					// clean up
				cleanup:
					dwCount = 0;
					RtlZeroMemory(collection, sizeof(collection));
					//memset(collection, 0, sizeof(collection));	
					bSkip = TRUE;
				}
			}
		}
		RtlZeroMemory(lpBuffer, BUFFERSIZE);

	} while (TRUE); 

	printf("top:\t\t%d\n", top.dwFirst);
	printf("second:\t\t%d\n", top.dwSecond);
	printf("third:\t\t%d\n", top.dwThird);

	DWORD dwTopThreeSum = top.dwFirst + top.dwSecond + top.dwThird;

	printf("final sum of top 3:\t%d", dwTopThreeSum);
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