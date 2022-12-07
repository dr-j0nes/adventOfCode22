#include "Shared.h"

void errorPrinter(LPTSTR printer)
{
DWORD dwError = 0;
dwError = GetLastError();
printf("%ls\n", printer);
printf("ERROR\t%d\n", dwError);
}