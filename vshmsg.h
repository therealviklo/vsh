#pragma once
#include <stdio.h>
#include "ps.h"

typedef enum
{
	MCLR_NOTICE,
	MCLR_ERROR
} VshMsgClr;

void vshMsg(const char* message, VshMsgClr colour);