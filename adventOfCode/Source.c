#include "Shared.h"
//
// need to be static for REASONS
// 0x1000 = 4096 = 1 page (ok not always but always)
//

int main() {
	DWORD retVal = 1111;

	retVal = day1();

	return retVal;
}