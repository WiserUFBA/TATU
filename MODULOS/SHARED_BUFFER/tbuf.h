/* tbuffer.h ~ Tatu Buffer Types and Expansion Macros */

#define BUFFER_1_BLOCKS(tipo, qtd)  tipo BUFFER_0[qtd]; \
                                    tipo BUFFER_1[qtd]
#define BUFFER_2_BLOCKS(tipo, qtd)  BUFFER_1_BLOCKS(tipo, qtd); \
                                    tipo BUFFER_2[qtd]; \
                                    tipo BUFFER_3[qtd]
#define BUFFER_4_BLOCKS(tipo, qtd)  BUFFER_2_BLOCKS(tipo, qtd); \
                                    tipo BUFFER_4[qtd]; \
                                    tipo BUFFER_5[qtd]; \
                                    tipo BUFFER_6[qtd]; \
                                    tipo BUFFER_7[qtd]
#define BUFFER_8_BLOCKS(tipo, qtd)  BUFFER_4_BLOCKS(tipo, qtd); \
                                    tipo BUFFER_8[qtd]; \
                                    tipo BUFFER_9[qtd]; \
                                    tipo BUFFER_A[qtd]; \
                                    tipo BUFFER_B[qtd]; \
                                    tipo BUFFER_C[qtd]; \
                                    tipo BUFFER_D[qtd]; \
                                    tipo BUFFER_E[qtd]; \
                                    tipo BUFFER_F[qtd]
#define BUFFER_16_BLOCKS(tipo,qtd)  BUFFER_8_BLOCKS(tipo, qtd); \
                                    tipo BUFFER_G[qtd]; \
                                    tipo BUFFER_H[qtd]; \
                                    tipo BUFFER_I[qtd]; \
                                    tipo BUFFER_J[qtd]; \
                                    tipo BUFFER_K[qtd]; \
                                    tipo BUFFER_L[qtd]; \
                                    tipo BUFFER_M[qtd]; \
                                    tipo BUFFER_N[qtd]; \
                                    tipo BUFFER_O[qtd]; \
                                    tipo BUFFER_P[qtd]; \
                                    tipo BUFFER_Q[qtd]; \
                                    tipo BUFFER_R[qtd]; \
                                    tipo BUFFER_S[qtd]; \
                                    tipo BUFFER_T[qtd]; \
                                    tipo BUFFER_U[qtd]; \
                                    tipo BUFFER_V[qtd]
#define BUFFER_32_BLOCKS(tipo,qtd)  tipo BUFFER_A0[qtd]; \
                                    tipo BUFFER_A1[qtd]; \
                                    tipo BUFFER_A2[qtd]; \
                                    tipo BUFFER_A3[qtd]; \
                                    tipo BUFFER_A4[qtd]; \
                                    tipo BUFFER_A5[qtd]; \
                                    tipo BUFFER_A6[qtd]; \
                                    tipo BUFFER_A7[qtd]; \
                                    tipo BUFFER_A8[qtd]; \
                                    tipo BUFFER_A9[qtd]; \
                                    tipo BUFFER_AA[qtd]; \
                                    tipo BUFFER_AB[qtd]; \
                                    tipo BUFFER_AC[qtd]; \
                                    tipo BUFFER_AD[qtd]; \
                                    tipo BUFFER_AE[qtd]; \
                                    tipo BUFFER_AF[qtd]; \
                                    tipo BUFFER_AG[qtd]; \
                                    tipo BUFFER_AH[qtd]; \
                                    tipo BUFFER_AI[qtd]; \
                                    tipo BUFFER_AJ[qtd]; \
                                    tipo BUFFER_AK[qtd]; \
                                    tipo BUFFER_AL[qtd]; \
                                    tipo BUFFER_AM[qtd]; \
                                    tipo BUFFER_AN[qtd]; \
                                    tipo BUFFER_AO[qtd]; \
                                    tipo BUFFER_AP[qtd]; \
                                    tipo BUFFER_AQ[qtd]; \
                                    tipo BUFFER_AR[qtd]; \
                                    tipo BUFFER_AS[qtd]; \
                                    tipo BUFFER_AT[qtd]; \
                                    tipo BUFFER_AU[qtd]; \
                                    tipo BUFFER_AV[qtd]; \
                                    tipo BUFFER_B0[qtd]; \
                                    tipo BUFFER_B1[qtd]; \
                                    tipo BUFFER_B2[qtd]; \
                                    tipo BUFFER_B3[qtd]; \
                                    tipo BUFFER_B4[qtd]; \
                                    tipo BUFFER_B5[qtd]; \
                                    tipo BUFFER_B6[qtd]; \
                                    tipo BUFFER_B7[qtd]; \
                                    tipo BUFFER_B8[qtd]; \
                                    tipo BUFFER_B9[qtd]; \
                                    tipo BUFFER_BA[qtd]; \
                                    tipo BUFFER_BB[qtd]; \
                                    tipo BUFFER_BC[qtd]; \
                                    tipo BUFFER_BD[qtd]; \
                                    tipo BUFFER_BE[qtd]; \
                                    tipo BUFFER_BF[qtd]; \
                                    tipo BUFFER_BG[qtd]; \
                                    tipo BUFFER_BH[qtd]; \
                                    tipo BUFFER_BI[qtd]; \
                                    tipo BUFFER_BJ[qtd]; \
                                    tipo BUFFER_BK[qtd]; \
                                    tipo BUFFER_BL[qtd]; \
                                    tipo BUFFER_BM[qtd]; \
                                    tipo BUFFER_BN[qtd]; \
                                    tipo BUFFER_BO[qtd]; \
                                    tipo BUFFER_BP[qtd]; \
                                    tipo BUFFER_BQ[qtd]; \
                                    tipo BUFFER_BR[qtd]; \
                                    tipo BUFFER_BS[qtd]; \
                                    tipo BUFFER_BT[qtd]; \
                                    tipo BUFFER_BU[qtd]; \
                                    tipo BUFFER_BV[qtd]

/* BUFFER DEFINES */
// One Byte Buffer
typedef struct{
    uint8_t BUFFER[256];
} BUFFER_UINT_SIZE_256;

typedef struct{
    // A
    BUFFER_1_BLOCKS(uint8_t, 128);
} BUFFER_UINT_SIZE_128;

typedef struct{
    // A | B
    BUFFER_2_BLOCKS(uint8_t, 64);
} BUFFER_UINT_SIZE_64;

typedef struct{
    // A | B | C | D
    BUFFER_4_BLOCKS(uint8_t, 32);
} BUFFER_UINT_SIZE_32;

typedef struct{
    // A | B | C | D | E | F | G | H
    BUFFER_8_BLOCKS(uint8_t, 16);
} BUFFER_UINT_SIZE_16;

typedef struct{
    // A | B | C | D | E | F | G | H
    // I | J | K | L | M | N | O | P
    BUFFER_16_BLOCKS(uint8_t, 8);
} BUFFER_UINT_SIZE_8;

typedef struct{
    /* Part A */
    // A | B | C | D | E | F | G | H
    // I | J | K | L | M | N | O | P
    /* Part B */
    // A | B | C | D | E | F | G | H
    // I | J | K | L | M | N | O | P
    BUFFER_32_BLOCKS(uint8_t, 4);
} BUFFER_UINT_SIZE_4;

// Two Byte Buffer
typedef struct{
    uint16_t BUFFER[128];
} BUFFER_UINT2_SIZE_128;

typedef struct{
    // A
    BUFFER_1_BLOCKS(uint16_t, 64);
} BUFFER_UINT2_SIZE_64;

typedef struct{
    // A | B
    BUFFER_2_BLOCKS(uint16_t, 32);
} BUFFER_UINT2_SIZE_32;

typedef struct{
   // A | B | C | D
    BUFFER_4_BLOCKS(uint16_t, 16);
} BUFFER_UINT2_SIZE_16;

typedef struct{
    // A | B | C | D | E | F | G | H
    BUFFER_8_BLOCKS(uint16_t, 8);
} BUFFER_UINT2_SIZE_8;

typedef struct {
    // A | B | C | D | E | F | G | H
    // I | J | K | L | M | N | O | P
    BUFFER_16_BLOCKS(uint16_t, 4);
} BUFFER_UINT2_SIZE_4;

// Four Byte Buffer
typedef struct{
    uint32_t BUFFER[64];
} BUFFER_UINT4_SIZE_64;

typedef struct{
    // A
    BUFFER_1_BLOCKS(uint32_t, 32);
} BUFFER_UINT4_SIZE_32;

typedef struct{
    // A | B
    BUFFER_2_BLOCKS(uint32_t, 16);
} BUFFER_UINT4_SIZE_16;

typedef struct{
   // A | B | C | D
    BUFFER_4_BLOCKS(uint32_t, 8);
} BUFFER_UINT4_SIZE_8;

typedef struct{
    // A | B | C | D | E | F | G | H
    BUFFER_8_BLOCKS(uint32_t, 4);
} BUFFER_UINT2_SIZE_4;
