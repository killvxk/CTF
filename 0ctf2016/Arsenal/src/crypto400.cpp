#include<iostream>
#include <ctime>
#include <string.h>
#include "AESFast.h"

void output( unsigned char * input, size_t len )
{
	for ( size_t i = 0; i < len; ++i )
	{
		printf("%02x ", input[i]);
	}
	puts("");

}

void read_key(unsigned char *key)
{
	FILE * fp = fopen("key.txt", "rb");
	fread( key, 1, 16, fp );
    fclose(fp);
}

void read_flag(unsigned char *flag)
{
	FILE * fp = fopen("flag.txt", "r");
	fread( flag, 1, 32, fp );
    fclose(fp);
}

void enc( const AESKey & encKey, unsigned char *zero, unsigned char *one, size_t ctr, time_t timer )
{
    memset( zero, 0, 16 );
    memset( one, 1, 16 );

	memcpy( zero + 8, &ctr, 4 );
	memcpy( one + 8, &ctr, 4 );
	memcpy( zero + 12, &timer, 4 );
	memcpy( one + 12, &timer, 4 );
	

	AES128_block_enc( zero, encKey );
	AES128_block_enc( one, encKey );
}

int main()
{
	unsigned char flag[32] = {0};
	read_flag(flag);
	
    unsigned char key[16] = {0};
    read_key(key);

	AESKey encKey;
	AES128_Key_Expansion( key, encKey );

	unsigned char zero[16];
    unsigned char one[16];
    
    size_t counter = 0;
    time_t timer = time(0);
    for ( size_t ctr = 0; ctr < 0x40; ++ctr )
    {
		enc( encKey, zero, one, ctr, timer );

		unsigned int coin = rand() % 2;
		output( coin == 0 ? zero : one, BLOCK_SIZE );

		puts("0 or 1?");
	    unsigned char choice = getchar() - 0x30;
	
		puts("ciphertext for 0 is: ");	
		output( zero, BLOCK_SIZE );
		puts("ciphertext for 1 is: ");	
		output( one, BLOCK_SIZE );

		if ( choice == coin )
		{
			puts("Correct!");
			++counter;
		}
		else
		{
			puts( "Incorrect!" );
		}
	}
	
	if ( 0x40 == counter )
	{
		puts("Now I will give you the flag:");
		AES128_block_enc( flag, encKey );
		AES128_block_enc( flag + 16, encKey );
		output(flag, sizeof(flag));
	}
}
