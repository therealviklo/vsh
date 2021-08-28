#pragma once

extern const unsigned char charsizes[262144];

static unsigned char getcharsize(unsigned c)
{
	if (c < 0x40000) return charsizes[c];
	if (c >= 0xe0000 && c <= 0xeffff) return 0;
	return 1;
}