#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MD5.h"
// General Constants
#define OUTPUT_HASH_SIZE    16
#define STD_CHANGE_SIZE     16
#define PASS_KEY_SIZE       20
#define MD5_PAD_SIZE        64
// General Macros
#define XOR_IN(a)    (a ^ 0x36)
#define XOR_OUT(a)   (a ^ 0x5C)
#define PRINT_HEX(a) printf("%c%c ", MAP_BASE16[(a >> 4)], MAP_BASE16[(a & 0x0F)])
// UNION
/*
    union {
        struct KEY {
            char KEY_INNER[65];
            char KEY_OUTER[65];
        }
        char DES_HASH[130];
    } THING_MEMORY;
*/

// Base16 MAP ~ should be in PROGMEM or maybe not
const char MAP_BASE16[] = "0123456789ABCDEF";

// PASSWORD SETTINGS
char PASS_KEY  [PASS_KEY_SIZE];
char STD_CHANGE[STD_CHANGE_SIZE];
// HMAC_MD5
char KEY_INNER[MD5_PAD_SIZE + 1];
char KEY_OUTER[MD5_PAD_SIZE + 1];
char OUTPUT_HASH[OUTPUT_HASH_SIZE];

int SALT = 0x14;
int KEY_LEN;
int STD_LEN;
int TMP;
int i, j;

void HMAC_MD5(){
    // Finish the strings with a '\0'
    KEY_INNER[MD5_PAD_SIZE] = 0;
    KEY_OUTER[MD5_PAD_SIZE] = 0;

    // XOR key with Pads
    for(i = 0, KEY_INNER[0] = 0, KEY_OUTER[0] = 0; i < MD5_PAD_SIZE; i++){
        KEY_INNER[i]  = (i < KEY_LEN ? XOR_IN (PASS_KEY[i]) : 0);
        KEY_OUTER[i] =  (i < KEY_LEN ? XOR_OUT(PASS_KEY[i]) : 0);
    }

    // Perform INNER MD5
    MD5Init();
    MD5Update();
    MD5Update();
    MD5Final();
    // Perform OUTER MD5
    MD5Init();
    MD5Update();
    MD5Update();
    MD5Final();
}

int main(){
    // Input for debug
    scanf("%s", PASS_KEY);
    scanf("%*c");
    scanf("%s", STD_CHANGE);
    // Important Lengths
    KEY_LEN = strlen(PASS_KEY);
    STD_LEN = strlen(STD_CHANGE);

    HMAC_MD5();

    for(i = 0; i < OUTPUT_HASH_SIZE; i++){
        TMP = OUTPUT_HASH[i];
        PRINT_HEX(TMP);
    }

    return EXIT_SUCCESS;
}
