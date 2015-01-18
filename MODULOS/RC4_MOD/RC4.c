#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// General Defines
#define SRC_PASS_SIZE	17
#define SRC_DATA_SIZE	17
#define OUT_HASH_SIZE	17
#define RC4_KEY_SIZE	256
// General Macros
#define SWAP(a,b) TMP = b; b = a; a = TMP
#define MOD_KEY(a, b) ((a + b) % RC4_KEY_SIZE)
#define OUTPUT_HASH(a, b, c) (SRC_DATA[c] ^ DES_HASH[MOD_KEY(DES_HASH[a], DES_HASH[b])])

// General variables
char SRC_PASS[SRC_PASS_SIZE];
char SRC_DATA[SRC_DATA_SIZE];
//unsigned char OUT_HASH[OUT_HASH_SIZE];
unsigned char DES_HASH[RC4_KEY_SIZE];

// Base16 MAP ~ should be in PROGMEM or maybe not
char MAP_BASE16[] = "0123456789ABCDEF";

//uint8_t TMP;
//uint8_t i, j, k;
int TMP, i, j, k;

void rc4_enc_dec(){
	for(i = 0; i < RC4_KEY_SIZE; i++)
		DES_HASH[i] = i;

	for(i = 0, j = 0; i < RC4_KEY_SIZE; i++){
		j = ((j + DES_HASH[i] + SRC_PASS[i % strlen(SRC_PASS)]) % RC4_KEY_SIZE);
		SWAP(DES_HASH[i], DES_HASH[j]);
	}

	for(i = 0, j = 0, k = 0; k < strlen(SRC_DATA); k++){
		i = MOD_KEY(i, 1);
		j = MOD_KEY(j, DES_HASH[i]);
		SWAP(DES_HASH[i], DES_HASH[j]);
		// OUTPUT
		TMP = OUTPUT_HASH(i, j, k);
		// PRINT BYTE H => MAP_BASE16[(TMP >> 4)]
		// PRINT BYTE L => MAP_BASE16[(TMP & 0x0F)]
		printf("%c%c ", MAP_BASE16[(TMP >> 4)], MAP_BASE16[(TMP & 0x0F)]);
	}
}

int main(){
	scanf("%s",SRC_PASS);
	scanf("%*c");
	scanf("%s",SRC_DATA);
	rc4_enc_dec();

	printf("\n");

	return EXIT_SUCCESS;
}
