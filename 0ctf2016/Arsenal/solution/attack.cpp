#include<iostream>
#include <ctime>
#include <string.h>
#include "AESFast.h"


const static size_t AMOUNT = 63;
const static size_t BLOCK = 0x10;

bool impossible ( unsigned char * buffer, size_t pos )
{
	unsigned char flags[0x100];
	memset(flags, 0, 0x100);
	
	for ( size_t i = 0; i < AMOUNT; ++i )
	{
		if ( flags[ buffer[i * BLOCK + pos] ] == 0 )
			flags[ buffer[i * BLOCK + pos] ] = 1;
		else
			return false;
	}

	return true;
}

#include "cipher.h"

int main()
{
	unsigned char buffer[BLOCK * AMOUNT];
	unsigned char lastKey[BLOCK] = {0};
	
	uint128 out;
	const static uint128 empty;
	
	time_t timer = time(0);
	for ( size_t i = 0x00000000; i < 0xffffffff; ++i )
	{
		lastKey[0] = (i >> 24);
		lastKey[7] = (i >> 16);
		lastKey[10] = (i >> 8);
		lastKey[13] = (i);
		lastKey[1] = (i >> 24);
		lastKey[4] = (i >> 16);
		lastKey[11] = (i >> 8);
		lastKey[14] = (i);
		lastKey[2] = (i >> 24);
		lastKey[5] = (i >> 16);
		lastKey[8] = (i >> 8);
		lastKey[15] = (i);
		lastKey[3] = (i >> 24);
		lastKey[6] = (i >> 16);
		lastKey[9] = (i >> 8);
		lastKey[12] = (i);

		for ( size_t j = 0; j < AMOUNT; ++j )
		{
			out = _mm_xor_si128( _mm_loadu_si128( (uint128 *)(zero + j * BLOCK) ), _mm_loadu_si128( (uint128 *)(lastKey) ) );
			out = _mm_aesdec_si128( out, empty );
			_mm_storeu_si128( (uint128 *)(buffer + j * BLOCK), out );
		}
		if ( impossible( buffer, 0 ) && impossible( buffer, 1 )  && impossible( buffer, 2 ) && impossible( buffer, 3 ))
		{
			printf( "key[0, 7, 10, 13] is: %08x\n", i );
		}
		if ( impossible( buffer, 4 ) && impossible( buffer, 5 )  && impossible( buffer, 6 ) && impossible( buffer, 7 ))
		{
			printf( "key[1, 4, 11, 14] is: %08x\n", i );
		}
		if ( impossible( buffer, 8 ) && impossible( buffer, 9 )  && impossible( buffer, 10 ) && impossible( buffer, 11 ))
		{
			printf( "key[2, 5, 8, 15] is: %08x\n", i );
		}
		if ( impossible( buffer, 12 ) && impossible( buffer, 13 )  && impossible( buffer, 14 ) && impossible( buffer, 15 ))
		{
			printf( "key[3, 6, 9, 12] is: %08x\n", i );
		}
	}
	
	printf("%d minutes\n", (time(0) - timer) / 60 );
}
