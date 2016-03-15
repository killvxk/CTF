#include<iostream>
#include <ctime>
#include <string.h>
#include "AESFast.h"

bool balance ( unsigned char * buffer, size_t pos )
{
	unsigned char z = 0;
	for ( size_t i = 0; i < 0x100; ++i )
	{
		z ^= Sboxinv[ buffer[pos + i * 0x10] ]; // not z ^= buffer[pos + i * 0x10];
	}

	return z == 0;
}

const static size_t AMOUNT = 0x200;
const static size_t BLOCK = 0x10;

#include "cipher.h"

int main()
{
	unsigned char buffer[BLOCK * AMOUNT];
	unsigned char lastKey[BLOCK] = {0};
	
	uint128 out;
	const static uint128 empty;
	
	for ( size_t pos = 0; pos < 16; ++pos )
	{
		for ( size_t i = 0; i < 0x100; ++i )
		{
			lastKey[pos] = i;
			for ( size_t j = 0; j < AMOUNT; ++j )
			{
				out = _mm_xor_si128( _mm_loadu_si128( (uint128 *)(zero + j * BLOCK) ), _mm_loadu_si128( (uint128 *)(lastKey) ) );
				_mm_storeu_si128( (uint128 *)(buffer + j * BLOCK), out );
			}
			if ( balance( buffer, pos ) && balance( buffer + 0x1000, pos ) )
			{
				printf( "key[%02x] is: %02x\n", pos, lastKey[pos] );
			}
		}
	}
}
