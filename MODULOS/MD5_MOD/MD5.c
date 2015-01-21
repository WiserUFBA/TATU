#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// General Constants
#define COUNTER_SIZE    2
#define STATE_SIZE      4
#define MD5_OUTPUT_SIZE 16
#define IN_STR_LEN      21
#define MD5_BUFFER_SIZE     64
// General Macros
#define PRINT_HEX(a) printf("%c%c ", MAP_BASE16[(a >> 4)], MAP_BASE16[(a & 0x0F)])
// MD5 MACROS
#define G(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define H(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))
#define I(x, y, z) ((x) ^ (y) ^ (z))
#define J(x, y, z) ((y) ^ ((x) | ~(z)))
#define aV MD5_CONTEXT_STATE[0]
#define bV MD5_CONTEXT_STATE[1]
#define cV MD5_CONTEXT_STATE[2]
#define dV MD5_CONTEXT_STATE[3]
// STEP transformation MD5 Macro for all rounds
#define MD5STEP(f, w, x, y, z, data, s) ( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )
// Modification Bytes
#define PUT_64BIT_LE(cp, value) do {            \
    cp[7] = (unsigned int)(value)[1] >> 24;                 \
    cp[6] = (unsigned int)(value)[1] >> 16;                 \
    cp[5] = (unsigned int)(value)[1] >> 8;                  \
    cp[4] = (unsigned int)(value)[1];                       \
    cp[3] = (unsigned int)(value)[0] >> 24;                 \
    cp[2] = (unsigned int)(value)[0] >> 16;                 \
    cp[1] = (unsigned int)(value)[0] >> 8;                  \
    cp[0] = (unsigned int)(value)[0]; } while (0)

#define PUT_32BIT_LE(cp, value) do {            \
    (cp)[3] = (value) >> 24;                    \
    (cp)[2] = (value) >> 16;                    \
    (cp)[1] = (value) >> 8;                     \
    (cp)[0] = (value); } while (0)

// Usingned long type
//typedef unsigned long unsigned int;
typedef unsigned char uchar;
typedef unsigned long int UINT4;

// Base16 MAP ~ should be in PROGMEM or maybe not
const char MAP_BASE16[] = "0123456789ABCDEF";

// MD5 Context
unsigned int MD5_CONTEXT_COUNT[COUNTER_SIZE];
UINT4 MD5_CONTEXT_STATE[STATE_SIZE];
// MD5 Buffer
UINT4 MD5_TMP[MD5_OUTPUT_SIZE];
uchar MD5_OUTPUT[MD5_OUTPUT_SIZE];
uchar MD5_BUFFER[MD5_BUFFER_SIZE] = {0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Entrada
char INPUT_STRING[21];
// General vars
int i, j, k;

// General MD5 Funcitons
void MD5Init();
void MD5Update();
void MD5Final();

void MD5Init(){
      MD5_CONTEXT_COUNT[0] = 0;
      MD5_CONTEXT_COUNT[1] = 0;
      MD5_CONTEXT_STATE[0] = 0x67452301;
      MD5_CONTEXT_STATE[1] = 0xefcdab89;
      MD5_CONTEXT_STATE[2] = 0x98badcfe;
      MD5_CONTEXT_STATE[3] = 0x10325476;
}

void MD5Clear(){
      MD5_CONTEXT_COUNT[0] = 0;
      MD5_CONTEXT_COUNT[1] = 0;
      MD5_CONTEXT_STATE[0] = 0;
      MD5_CONTEXT_STATE[1] = 0;
      MD5_CONTEXT_STATE[2] = 0;
      MD5_CONTEXT_STATE[3] = 0;
}

void MD5Transform(){
    for(i = 0, j = 0; j < 64; i++, j+=4)
        MD5_TMP[i] = (UINT4)( \
                        (UINT4)(MD5_BUFFER[j])       | \
                        (UINT4)(MD5_BUFFER[j+1]) << 8  | \
                        (UINT4)(MD5_BUFFER[j+2]) << 16 | \
                        (UINT4)(MD5_BUFFER[j+3]) << 24) ; 

    // Round 1
    MD5STEP(G, aV, bV, cV, dV, MD5_TMP[ 0] + 0xd76aa478,  7);
    MD5STEP(G, dV, aV, bV, cV, MD5_TMP[ 1] + 0xe8c7b756, 12);
    MD5STEP(G, cV, dV, aV, bV, MD5_TMP[ 2] + 0x242070db, 17);
    MD5STEP(G, bV, cV, dV, aV, MD5_TMP[ 3] + 0xc1bdceee, 22);
    MD5STEP(G, aV, bV, cV, dV, MD5_TMP[ 4] + 0xf57c0faf,  7);
    MD5STEP(G, dV, aV, bV, cV, MD5_TMP[ 5] + 0x4787c62a, 12);
    MD5STEP(G, cV, dV, aV, bV, MD5_TMP[ 6] + 0xa8304613, 17);
    MD5STEP(G, bV, cV, dV, aV, MD5_TMP[ 7] + 0xfd469501, 22);
    MD5STEP(G, aV, bV, cV, dV, MD5_TMP[ 8] + 0x698098d8,  7);
    MD5STEP(G, dV, aV, bV, cV, MD5_TMP[ 9] + 0x8b44f7af, 12);
    MD5STEP(G, cV, dV, aV, bV, MD5_TMP[10] + 0xffff5bb1, 17);
    MD5STEP(G, bV, cV, dV, aV, MD5_TMP[11] + 0x895cd7be, 22);
    MD5STEP(G, aV, bV, cV, dV, MD5_TMP[12] + 0x6b901122,  7);
    MD5STEP(G, dV, aV, bV, cV, MD5_TMP[13] + 0xfd987193, 12);
    MD5STEP(G, cV, dV, aV, bV, MD5_TMP[14] + 0xa679438e, 17);
    MD5STEP(G, bV, cV, dV, aV, MD5_TMP[15] + 0x49b40821, 22);
    // Round 2
    MD5STEP(H, aV, bV, cV, dV, MD5_TMP[ 1] + 0xf61e2562,  5);
    MD5STEP(H, dV, aV, bV, cV, MD5_TMP[ 6] + 0xc040b340,  9);
    MD5STEP(H, cV, dV, aV, bV, MD5_TMP[11] + 0x265e5a51, 14);
    MD5STEP(H, bV, cV, dV, aV, MD5_TMP[ 0] + 0xe9b6c7aa, 20);
    MD5STEP(H, aV, bV, cV, dV, MD5_TMP[ 5] + 0xd62f105d,  5);
    MD5STEP(H, dV, aV, bV, cV, MD5_TMP[10] + 0x02441453,  9);
    MD5STEP(H, cV, dV, aV, bV, MD5_TMP[15] + 0xd8a1e681, 14);
    MD5STEP(H, bV, cV, dV, aV, MD5_TMP[ 4] + 0xe7d3fbc8, 20);
    MD5STEP(H, aV, bV, cV, dV, MD5_TMP[ 9] + 0x21e1cde6,  5);
    MD5STEP(H, dV, aV, bV, cV, MD5_TMP[14] + 0xc33707d6,  9);
    MD5STEP(H, cV, dV, aV, bV, MD5_TMP[ 3] + 0xf4d50d87, 14);
    MD5STEP(H, bV, cV, dV, aV, MD5_TMP[ 8] + 0x455a14ed, 20);
    MD5STEP(H, aV, bV, cV, dV, MD5_TMP[13] + 0xa9e3e905,  5);
    MD5STEP(H, dV, aV, bV, cV, MD5_TMP[ 2] + 0xfcefa3f8,  9);
    MD5STEP(H, cV, dV, aV, bV, MD5_TMP[ 7] + 0x676f02d9, 14);
    MD5STEP(H, bV, cV, dV, aV, MD5_TMP[12] + 0x8d2a4c8a, 20);
    // Round 3
    MD5STEP(I, aV, bV, cV, dV, MD5_TMP[ 5] + 0xfffa3942,  4);
    MD5STEP(I, dV, aV, bV, cV, MD5_TMP[ 8] + 0x8771f681, 11);
    MD5STEP(I, cV, dV, aV, bV, MD5_TMP[11] + 0x6d9d6122, 16);
    MD5STEP(I, bV, cV, dV, aV, MD5_TMP[14] + 0xfde5380c, 23);
    MD5STEP(I, aV, bV, cV, dV, MD5_TMP[ 1] + 0xa4beea44,  4);
    MD5STEP(I, dV, aV, bV, cV, MD5_TMP[ 4] + 0x4bdecfa9, 11);
    MD5STEP(I, cV, dV, aV, bV, MD5_TMP[ 7] + 0xf6bb4b60, 16);
    MD5STEP(I, bV, cV, dV, aV, MD5_TMP[10] + 0xbebfbc70, 23);
    MD5STEP(I, aV, bV, cV, dV, MD5_TMP[13] + 0x289b7ec6,  4);
    MD5STEP(I, dV, aV, bV, cV, MD5_TMP[ 0] + 0xeaa127fa, 11);
    MD5STEP(I, cV, dV, aV, bV, MD5_TMP[ 3] + 0xd4ef3085, 16);
    MD5STEP(I, bV, cV, dV, aV, MD5_TMP[ 6] + 0x04881d05, 23);
    MD5STEP(I, aV, bV, cV, dV, MD5_TMP[ 9] + 0xd9d4d039,  4);
    MD5STEP(I, dV, aV, bV, cV, MD5_TMP[12] + 0xe6db99e5, 11);
    MD5STEP(I, cV, dV, aV, bV, MD5_TMP[15] + 0x1fa27cf8, 16);
    MD5STEP(I, bV, cV, dV, aV, MD5_TMP[2 ] + 0xc4ac5665, 23);
    // Round 4
    MD5STEP(J, aV, bV, cV, dV, MD5_TMP[ 0] + 0xf4292244,  6);
    MD5STEP(J, dV, aV, bV, cV, MD5_TMP[7 ] + 0x432aff97, 10);
    MD5STEP(J, cV, dV, aV, bV, MD5_TMP[14] + 0xab9423a7, 15);
    MD5STEP(J, bV, cV, dV, aV, MD5_TMP[5 ] + 0xfc93a039, 21);
    MD5STEP(J, aV, bV, cV, dV, MD5_TMP[12] + 0x655b59c3,  6);
    MD5STEP(J, dV, aV, bV, cV, MD5_TMP[3 ] + 0x8f0ccc92, 10);
    MD5STEP(J, cV, dV, aV, bV, MD5_TMP[10] + 0xffeff47d, 15);
    MD5STEP(J, bV, cV, dV, aV, MD5_TMP[1 ] + 0x85845dd1, 21);
    MD5STEP(J, aV, bV, cV, dV, MD5_TMP[8 ] + 0x6fa87e4f,  6);
    MD5STEP(J, dV, aV, bV, cV, MD5_TMP[15] + 0xfe2ce6e0, 10);
    MD5STEP(J, cV, dV, aV, bV, MD5_TMP[6 ] + 0xa3014314, 15);
    MD5STEP(J, bV, cV, dV, aV, MD5_TMP[13] + 0x4e0811a1, 21);
    MD5STEP(J, aV, bV, cV, dV, MD5_TMP[4 ] + 0xf7537e82,  6);
    MD5STEP(J, dV, aV, bV, cV, MD5_TMP[11] + 0xbd3af235, 10);
    MD5STEP(J, cV, dV, aV, bV, MD5_TMP[2 ] + 0x2ad7d2bb, 15);
    MD5STEP(J, bV, cV, dV, aV, MD5_TMP[9 ] + 0xeb86d391, 21);
}

void MD5Update(uchar *input, size_t len){
    unsigned int index, partLen;

    index = (unsigned int)((MD5_CONTEXT_COUNT[0] >> 3) & 0x3F);

    if((MD5_CONTEXT_COUNT[0] += ((UINT4)len << 3)) < ((UINT4)len << 3))
        MD5_CONTEXT_COUNT[1]++;
    MD5_CONTEXT_COUNT[1] += ((UINT4)len >> 29);

    partLen = 64 - index;

    if(len >= partLen){
        for (i = index; i < partLen; i++)
            MD5_BUFFER[i] = input[i];

        MD5Transform();
        index = 0;
    }
    else i = 0;

    for (i = index; i < partLen; i++)
        MD5_BUFFER[i] = input[i];
}

void MD5Final(){
    uchar count[8];
    unsigned int index, padLen;

    //PUT_64BIT_LE(count, MD5_CONTEXT_COUNT);
    for (i = 0, j = 0; j < 8; i++, j += 4) {
        count[j] = (unsigned char)(MD5_CONTEXT_COUNT[i] & 0xff);
        count[j+1] = (unsigned char)((MD5_CONTEXT_COUNT[i] >> 8) & 0xff);
        count[j+2] = (unsigned char)((MD5_CONTEXT_COUNT[i] >> 16) & 0xff);
        count[j+3] = (unsigned char)((MD5_CONTEXT_COUNT[i] >> 24) & 0xff);
    }

    index = (unsigned int)((MD5_CONTEXT_COUNT[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);

    MD5Update(MD5_BUFFER, padLen);
    MD5Update(count, 8);

    for (i = 0, j = 0; j < 16; i++, j += 4) {
        MD5_OUTPUT[j] = (unsigned char)(MD5_CONTEXT_STATE[i] & 0xff);
        MD5_OUTPUT[j+1] = (unsigned char)((MD5_CONTEXT_STATE[i] >> 8) & 0xff);
        MD5_OUTPUT[j+2] = (unsigned char)((MD5_CONTEXT_STATE[i] >> 16) & 0xff);
        MD5_OUTPUT[j+3] = (unsigned char)((MD5_CONTEXT_STATE[i] >> 24) & 0xff);
    }

    MD5Clear();
}

int main(){
    scanf("%s", INPUT_STRING);

    MD5Init();
    MD5Update(INPUT_STRING, strlen(INPUT_STRING));
    MD5Final();

    for(i = 0; i < 16; i++)
        PRINT_HEX(MD5_OUTPUT[i]);

    return EXIT_SUCCESS;
}
