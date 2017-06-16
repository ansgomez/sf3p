/************************************************************************
 * MJPEG-JFIF decoder                                                   *
 ************************************************************************/

#ifndef __JPEG_H__
#define __JPEG_H__

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define SILENT    0
#define VERBOSE   1
#define INFO      2
#define VERBOSITY SILENT

void dbgprintf(int urgency, const char* format, ...);

static const int ZIGZAG_COEFFS[64] = {
     0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};

int* unZigZag(int* dest, int* src);

int* unquantify(int* block, unsigned char* qtable);

//minimum and maximum values a `signed int' can hold.
#define INT_MAX2  2147483647
#define INT_MIN2  (-INT_MAX2 - 1)

//useful constants
//ck = cos(k*pi/16) = s8-k = sin((8-k)*pi/16) times 1 << C_BITS and rounded
#define c0_1   16384
#define c0_s2  23170
#define c1_1   16069
#define c1_s2  22725
#define c2_1   15137
#define c2_s2  21407
#define c3_1   13623
#define c3_s2  19266
#define c4_1   11585
#define c4_s2  16384
#define c5_1   9102
#define c5_s2  12873
#define c6_1   6270
#define c6_s2  8867
#define c7_1   3196
#define c7_s2  4520
#define c8_1   0
#define c8_s2  0
#define sqrt2  c0_s2

// the number of bits of accuracy in all (signed) integer operations:
// may lie between 1 and 32 (bounds inclusive).
#define ARITH_BITS      16

// the minimum signed integer value that fits in ARITH_BITS:
#define ARITH_MIN       (-1 << (ARITH_BITS-1))
// the maximum signed integer value that fits in ARITH_BITS:
#define ARITH_MAX       (~ARITH_MIN)

// the number of bits coefficients are scaled up before 2-D IDCT:
#define S_BITS           3
// the number of bits in the fractional part of a fixed point constant:
#define C_BITS          14

#define SCALE(x, n)     ((x) << (n))

/* This version is vital in passing overall mean error test. */
inline int DESCALE(int x, int n);
inline int ADD(int x, int y);
inline int SUB(int x, int y);
inline int CMUL(int c, int x);

/* Rotate (x,y) over angle k*pi/16 (counter-clockwise) and scale with f. */
inline void rot(int f, int k, int x, int y, int *rx, int *ry);

/* Butterfly: but(a,b,x,y) = rot(sqrt(2),4,a,b,x,y) */
#define but(a,b,x,y) { x = SUB(a,b); y = ADD(a,b); }

inline void idct_1d(int *Y);
void IDCT(int input[8][8], unsigned char output[8][8]);

int intceil(int N, int D);
int intfloor(int N, int D);
int reformat(unsigned long s, int good);

/* The tg use a TDMA schema to dispatch frames to output
   ports. The number of parallel ports is defined below */
#define NUM_OF_PAR_PORTS 1

// Number of frames, to end each process separately, especially
// for MPARM and VSP, where the simulation ends only when every process ends
#define NUMBER_OF_FRAMES 8  //1, 2, 31, 8

/* define maximum picture size */
#define MAX_WIDTH  320 //1024
#define MAX_HEIGHT 240 //1024
// maximum size of an encoded frame. should be multiple of 4 bytes
#define MAX_ENCODED_FRAME  10000

// define fixed MCU size (8)
#define MCU_sx     8
#define MCU_sy     8
#define MAX_YMCU   MAX_WIDTH/MCU_sy
#define MAX_XMCU   MAX_HEIGHT/MCU_sx
#define MAX_BLOCK  MAX_YMCU*MAX_XMCU
/* size of maximum send element in fifo */
/* NB_SEND * MAX_SEND = 64              */
#define MAX_SEND   64
#define NB_SEND    1


/*----------------------------------*/
/* JPEG format parsing markers here */
/*----------------------------------*/
#define SOI_MK     0xFFD8		/* start of image       */
#define APP_MK     0xFFE0		/* custom, up to FFEF   */
#define COM_MK     0xFFFE		/* commment segment     */
#define SOF_MK     0xFFC0		/* start of frame       */
#define SOS_MK     0xFFDA		/* start of scan        */
#define DHT_MK     0xFFC4		/* Huffman table        */
#define DQT_MK     0xFFDB		/* Quant. table	        */
#define DRI_MK     0xFFDD		/* restart interval     */
#define EOI_MK     0xFFD9		/* end of image         */
#define MK_MSK     0xFFF0
#define RST_MK(x)  ( (0xFFF8&(x)) == 0xFFD0 ) /* is x a restart interval ? */

/*-------------------------------------------------------- */
/* all kinds of macros here				*/
/*-------------------------------------------------------- */
#define first_quad(c)   ((c)>>4)        /* first 4 bits in file order */
#define second_quad(c)  ((c)&15)
#define HUFF_ID(class, id)  (2*(class)+(id))
#define DC_CLASS        0
#define AC_CLASS        1
#define CASS            1
#define MAX_SIZE(Mclass) ((Mclass)?162:14) /* Memory size of HTables */

/*-------------------------------------------------------*/
/* JPEG data types here					*/
/*-------------------------------------------------------*/
/* component descriptor structure */
typedef struct {
	unsigned char	CID;	/* component ID */
	char		QT;	/* QTable index, 2bits 	*/
	char		DC_HT;	/* DC table index, 1bit */
	char		AC_HT;	/* AC table index, 1bit */
	int		PRED;	/* DC predictor value */
	} cd_t;

/*--------------------------------------------*/
/* global variables here                      */
/*--------------------------------------------*/
#ifdef CASS
#define EOF             (-1)
#endif

/*-----------------------------------------*/
/* prototypes from utils.c		   */
/*-----------------------------------------*/
int intceil(int N, int D);
int intfloor(int N, int D);
int reformat(unsigned long S, int good);

/*-----------------------------------------*/
/* prototypes from parse.c		   */
/*-----------------------------------------*/
//parse3.c
//int get_bits2(const void *, int, unsigned long *);
//int get_one_bit2(const void *, unsigned char *);
//void clear_bits();
//inline unsigned int get_size3(const void *);

//parse.c
//inline unsigned int get_size2(const void *);
//unsigned int get_next_MK2(const void *);
//void skip_segment2(const void *);
//int load_quant_tables2(const void *, const void *);

/*-----------------------------------------*/
/* prototypes from table_vld.c or tree_vld.c */
/*-----------------------------------------*/
//int load_huff_tables2(const void *);
//int get_symbol2(const void *, int, unsigned char *);

/*-----------------------------------------*/
/* prototypes from huffman.c 		   */
/*-----------------------------------------*/
//int unpack_block2(const void *, const void *, cd_t *);

/*-----------------------------------------*/
/* command for VLD                     	   */
/*-----------------------------------------*/
#define DHT_CMD	0xFA  /* command Define Huffman Table for VLD */
#define SOS_CMD	0xFB  /* command Start Of Scan for VLD        */
#define RPS_CMD	0xFC  /* command Read Picture Size for VLD    */
#define ABR_CMD	0xFD  /* command abort picture                */

#endif	// __JPEG_H__
