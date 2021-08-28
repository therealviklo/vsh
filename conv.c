#include "conv.h"

unsigned utf8charcode(const char c[4])
{
	unsigned out = 0;
	for (int i = 0; i < 4; i++)
	{
		if (!c[i]) break;
		if ((unsigned char)c[i] < (unsigned char)0b10000000)
		{
			out = (out << 7) | (c[i] & 0b01111111);
		}
		else if ((unsigned char)c[i] < (unsigned char)0b11000000)
		{
			out = (out << 6) | (c[i] & 0b00111111);
		}
		else if ((unsigned char)c[i] < (unsigned char)0b11100000)
		{
			out = (out << 5) | (c[i] & 0b00011111);
		}
		else if ((unsigned char)c[i] < (unsigned char)0b11110000)
		{
			out = (out << 4) | (c[i] & 0b00001111);
		}
		else if ((unsigned char)c[i] < (unsigned char)0b11111000)
		{
			out = (out << 3) | (c[i] & 0b00000111);
		}
	}
	return out;
}