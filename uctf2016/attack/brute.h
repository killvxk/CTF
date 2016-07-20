#define EncByte(lastbyte, plain, key)	sbox[(unsigned char) (plain ^ lastbyte ^ key)];
#define DecByte(lastbyte, cipher, key)	(rsbox[(unsigned char) cipher] ^ lastbyte ^ key)


void DecRound(char *cipher, int cipherlen, char *plain, char *IV, char *key, int keylen)
{
	for(int i = cipherlen - 1; i >= 0; i--)
	{
		if(i == 0)
		{
			plain[i] = DecByte(*IV, cipher[i], key[i % keylen]);
		}
		else
		{
			plain[i] = DecByte(cipher[i - 1], cipher[i], key[i % keylen]);
		}
	}
}



void Decrypt(char *cipher, int cipherlen, char *plain, char *key, int keylen, int round)
{
	for(int r = round - 1; r >= 0; r--)
	{
		if(r == 0)
		{
			DecRound(cipher, cipherlen, plain, key, key + 1, keylen - 1);
		}
		else
		{
			DecRound(cipher, cipherlen, cipher, cipher + cipherlen - 1, key + 1, keylen - 1);
		}
	}
}
