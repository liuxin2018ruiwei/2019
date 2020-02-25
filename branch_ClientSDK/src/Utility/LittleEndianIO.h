#ifndef LITTLE_ENDIAN_IO_H__
#define LITTLE_ENDIAN_IO_H__


class LittleEndianIO
{
public:	
	static unsigned short int readInt16(const char* data);
	static int readInt32(const char* data);

	static void writeInt16( char* data, short int value);
	static void writeInt32(char* data, int value);

};


#endif
