#include "vshmsg.h"

void vshMsg(const char* message, VshMsgClr colour)
{
	switch (colour)
	{
		case MCLR_NOTICE:
		{
			setColour(CLR_DARKGREEN);
		}
		break;
		case MCLR_ERROR:
		{
			setColour(CLR_DARKRED);
		}
		break;
	}
	printf("vsh");
	setColour(CLR_WOB);
	printf(": %s\n", message);
}