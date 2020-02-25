#include "LittleEndianIO.h"


unsigned short int LittleEndianIO::readInt16(const char* data)
{
	unsigned int low = data[0];
	unsigned int high = data[1];
	return ((high << 8) | low);
}

int LittleEndianIO::readInt32(const char* data)
{
	unsigned int lowLow = data[0];
	unsigned int lowHigh = data[1];
	unsigned int highLow = data[2];
	unsigned int highHigh = data[3];
	return ((highHigh<<24)|(highLow<<16)|(lowHigh<<8)|lowLow);
}

void LittleEndianIO::writeInt16(char* data, short int value)
{
	data[0] = char(value & 0xff);
	data[1] = char((value >> 8) & 0xff);
}

void LittleEndianIO::writeInt32(char* data, int value)
{
	data[0] = char(value & 0xff);
	data[1] = char((value >> 8) & 0xff);
	data[2] = char((value >> 16) & 0xff);
	data[3] = char((value >> 24) & 0xff);

}

