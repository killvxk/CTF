#include <iostream>
#include "AESFast.h"

static const size_t ROUNDS = 4;

void AES_128_Assist (uint128 & l, uint128 & r)
{
	uint128 m;
	r = _mm_shuffle_epi32 (r ,0xff);
	m = _mm_slli_si128(l, 0x4);
	l = _mm_xor_si128(l, m);
	m = _mm_slli_si128(m, 0x4);
	l = _mm_xor_si128(l, m);
	m = _mm_slli_si128(m, 0x4);
	l = _mm_xor_si128(l, m);
	l = _mm_xor_si128(l, r);
}

void AES128_Key_Expansion(const uint8 key[KEY128_SIZE], AESKey & roundKeys)
{
	uint128 l, r;

	l = _mm_loadu_si128( (uint128 *)key );
	
	_mm_storeu_si128(roundKeys + 0 , l);
	r = _mm_aeskeygenassist_si128(l, 1);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 1 , l);
	r = _mm_aeskeygenassist_si128(l, 2);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 2 , l);
	r = _mm_aeskeygenassist_si128(l, 4);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 3 , l);
	r = _mm_aeskeygenassist_si128(l, 8);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 4 , l);
	r = _mm_aeskeygenassist_si128(l, 0x10);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 5 , l);
	r = _mm_aeskeygenassist_si128(l, 0x20);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 6 , l);
	r = _mm_aeskeygenassist_si128(l, 0x40);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 7 , l);
	r = _mm_aeskeygenassist_si128(l, 0x80);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 8 , l);
	r = _mm_aeskeygenassist_si128(l, 0x1b);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 9 , l);
	r = _mm_aeskeygenassist_si128(l, 0x36);
	AES_128_Assist(l, r);

	_mm_storeu_si128(roundKeys + 10, l);
}


void AES128_block_enc( unsigned char * p, const AESKey & encKey )
{
	uint128 out = _mm_xor_si128( _mm_loadu_si128( (uint128 *)(p) ), encKey[0] );
	for ( size_t i = 1; i < ROUNDS; ++i )
	{
		out = _mm_aesenc_si128(out, encKey[i]);
	}
	out = _mm_aesenclast_si128(out, encKey[ROUNDS]);
	_mm_storeu_si128((uint128 *)(p), out);
}



