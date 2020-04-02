#include "kark.h"

char KeToKernekModel(PKTHREAD Kthread)
{
	CHAR previousMode;

	previousMode = *(char*)PTR_ADD_OFFSET(Kthread, NT::KTHREAD::PreviousModeOffset);
	*(char*)PTR_ADD_OFFSET(Kthread, NT::KTHREAD::PreviousModeOffset) = 0;
	return previousMode;
}

void KeResumePreviousMode(PKTHREAD Kthread, CHAR PreviousMode)
{
	*(char*)PTR_ADD_OFFSET(Kthread, NT::KTHREAD::PreviousModeOffset) = PreviousMode;
}
