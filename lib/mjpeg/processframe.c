/************************************************************************
 * demultiplexing of blocks                                             *
 ************************************************************************/

#include "processframe.h"

#include <stdio.h>
#include <stdlib.h>

#define my_read(ptr, len, p) \
    do {                           \
        memcpy(ptr, p->fptr, len);      \
        p->fptr += len;                 \
        p->fLen -= len;                 \
    } while (0);                               \


int load_huff_tables(Processframe_State *p);
int unpack_block2(unsigned char block[8][8], Processframe_State *p, cd_t *comp);
inline unsigned int get_size2(Processframe_State *p);
void skip_segment2(Processframe_State *p);
unsigned int get_next_MK2(Processframe_State *p);

/*-------------------------------------------------*/
/* here we unpack                                  */
/* a complete 8*8 DCT block ...                    */
/*-------------------------------------------------*/
int get_bits2(Processframe_State *p, int number,
              unsigned long *res);
int get_symbol2(Processframe_State *p, int select,
                unsigned char *res);
int get_one_bit2(Processframe_State *p, unsigned char *res);

unsigned char *decodeFrame(struct _mjpeg_state *state, int fLen, unsigned char *frame, int *x_size, int *y_size) {

    Processframe_State *p = (Processframe_State *) malloc(sizeof(Processframe_State)); 
    p->vld.HTable[0] = p->vld.DC_Table0;
    p->vld.HTable[1] = p->vld.DC_Table1;
    p->vld.HTable[2] = p->vld.AC_Table0;
    p->vld.HTable[3] = p->vld.AC_Table1;
    p->num_iter = 0;

    char * ptr;
    int done;
    unsigned int aux, mark;
    unsigned char buf, waste;
    int in_frame; //frame started? current component?
    int found_MK; //if marker found while read data found_MK = 1
    int nblock; //picture size in number of MCUs
    int *data; 

    // read frame size and frame
    p->fptr = (char *) frame;

    //now process segments as they appear first find the SOI marker
    do {
        aux = get_next_MK2(p);
    } while (aux != SOI_MK);

    found_MK = 0; //marker already found
    done = 1;
    while (done) {
        if (found_MK == 0)  {
            mark = get_next_MK2(p);
        }

        switch (mark) {
        case SOF_MK: {

            in_frame = 1;
            
            found_MK = 0;
            //header size, don't care
            get_size2(p);

            //precision, 8bit, don't care
            my_read(&waste, 1*(sizeof(waste)), p);

            //load basic image parameters
            *y_size = get_size2(p);
            *x_size = get_size2(p);

            p->vld.x_size = *x_size;
            p->vld.y_size = *y_size;
            p->vld.mx_size = intceil(*x_size, MCU_sx);
            p->vld.my_size = intceil(*y_size, MCU_sy);

            data = (int *) malloc(p->vld.my_size * p->vld.x_size * MCU_sy); 
            
            //total number of MCU in picture
            nblock = (*y_size/MCU_sy)*(*x_size/MCU_sx);
            
            //number of components,don't care
            my_read(&waste, 1*(sizeof(waste)), p);

            //component order
            my_read(&buf, 1*(sizeof(buf)), p);

            //sampling factor, don't care
            my_read(&buf, 1*(sizeof(buf)), p);

            //quantization table index,don't care for jfif
            my_read(&buf, 1*(sizeof(buf)), p);
            break;
        }
        case DHT_MK: {
            //VLD: loading Huffman table
            load_huff_tables(p);

            //remove the rest
            while ((ptr = (char *)memchr(p->fptr,0xFF,p->fLen))
                   != NULL) {
                int offset;
                unsigned char pot_mark = *(ptr+1);
                if ((pot_mark != 0) && (pot_mark != 0xD8)) {
                    offset = ptr - p->fptr + 2;
                    p->fLen -= offset;
                    p->fptr += offset;
                    mark = (0xFF00|(unsigned int)pot_mark);
                    found_MK = 1;
                    break;
                }

                offset = ptr - p->fptr + 2;
                p->fLen -= offset;
                p->fptr += offset;
            }

            break;
                     }
        case DQT_MK:
            {
                unsigned char aux;
                
                get_size2(p); //this is the table's size
                my_read(&aux, 1*(sizeof(aux)), p);
                memcpy(p->vld.QTable, p->fptr, sizeof(p->vld.QTable));
                p->fptr += sizeof(p->vld.QTable);
            }
            break;

        case DRI_MK:
            {
            //skip size
            get_size2(p);
            get_size2(p);
            break;
            }

        case SOS_MK: {
            cd_t comp;    // descriptors for 3 components
            //int leftover; // RST check
            get_size2(p); // don't care
            get_size2(p); // don't care

            my_read(&buf, sizeof(buf), p);
            comp.DC_HT = first_quad(buf);
            comp.AC_HT = second_quad(buf);

            get_size2(p); // don't care
            my_read(&buf, sizeof(buf), p);

            p->vld.bit_count = 0; // initialise vld decoder
            comp.PRED = 0; //initialise vld predictor
            //leftover = p->vld.mx_size * p->vld.my_size;

            {
                int goodrows, goodcolumns;
                unsigned char *LineBuffer  = p->LineBuffer;

                goodrows = MCU_sy;
                goodcolumns = MCU_sx;

                //piece-wise processing

                int j, k, l; 
                for (j = 0; j < p->vld.my_size; j++) {
                    for (k = 0; k < p->vld.mx_size; k++) {
                        unsigned char block[8][8];
                        unpack_block2(block, p, &comp);
                        for (l = 0; l < goodrows; l++) {
                            memcpy(LineBuffer + k * MCU_sx + l * p->vld.x_size,
                                   &block[l][0], goodcolumns);
                        }
                    }
                    for (l = 0; l < (p->vld.x_size * MCU_sy) / 4; l++) {
                        data[j * ((p->vld.x_size * MCU_sy) / 4) + l] =(*(LineBuffer + 4 * l))
                            + (*(LineBuffer + 4 * l + 1) << 8)
                            + (*(LineBuffer + 4 * l + 2) << 16)
                            + (*(LineBuffer + 4 * l + 3) << 24);
                    }
                }
            }

            //remove the rest
            while ((ptr = (char *)memchr(p->fptr,0xFF,p->fLen))
                   != NULL) {
                int offset;
                unsigned char pot_mark = *(ptr+1);
                if ((pot_mark != 0) && (pot_mark != 0xD8)) {
                    offset = ptr - p->fptr + 2;
                    p->fLen -= offset;
                    p->fptr += offset;
                    mark = (0xFF00|(unsigned int)pot_mark);
                    found_MK = 1;
                    break;
                }
                offset = ptr - p->fptr + 2;
                p->fLen -= offset;
                p->fptr += offset;
            }

            in_frame = 0;
            break;

        }
        case EOI_MK:
            done = 0;
            break;
        case COM_MK:
            skip_segment2(p);
            break;

        case EOF:
            exit(0);
        default:
            if ((mark & MK_MSK) == APP_MK) {
                skip_segment2(p);
                break;
            }
            if (RST_MK(mark)) {
                break;
            }
            done = 0;
            break;
        }
    }

    free(p); 
    in_frame++;
    nblock++;
    return (unsigned char *) data;
}

int unpack_block2(unsigned char block[8][8],
                         Processframe_State *p, cd_t *comp) {
    unsigned long temp;
    unsigned int i, run, cat;
    int value;
    unsigned char symbol;
    int T[64];
    memset((void *)T, 0, sizeof(T)); //zeroize block
    int block2[8][8];

    //first get the DC coefficient
    get_symbol2(p, HUFF_ID(DC_CLASS,comp->DC_HT),&symbol);
    get_bits2(p, symbol,&temp);

    value = reformat(temp, symbol);
    value += comp->PRED;
    comp->PRED = value;

    //reoganize and unquantify -> move to ZZ and IQ
    T[0] = value ;

    //then the AC ones
    //if symbol found is EOB and process not finish, missing values are
    //replaced by zero
    for (i = 1; i < 64; i++) {
        get_symbol2(p, HUFF_ID(AC_CLASS, comp->AC_HT), &symbol);

        if (symbol == 0x00) break;
        if (symbol == 0xF0) {
            i += 15;
            continue;
        }
        cat = symbol & 15;
        run = (symbol >> 4) & 15;
        i += run;
        get_bits2(p, cat, &temp);
        value = reformat(temp, cat);
        T[i] = value ;

        //63 is to exit without EOB if last coef non-zero
        if (i == 63) break;
    }
    unquantify(T, p->vld.QTable);
    unZigZag((int*)block2, T);
    IDCT(block2, block);

    return 0;
}

//utility and counter to return the number of bits from file
//right aligned, masked, first bit towards MSB's
int get_bits2(Processframe_State *p, int number,
              unsigned long *res) {
    int i, newbit;
    unsigned long result = 0;
    unsigned char aux, wwindow;

    *(res) = 0;
    if (!number) return 0;
    for (i = 0; i < number; i++) {
        if (p->vld.bit_count == 0) {
            my_read(&wwindow, sizeof(wwindow), p);
            if (wwindow == 0xFF) {
                my_read(&aux, sizeof(aux), p);
                p->vld.bit_count = 0;
            }
            p->vld.bit_count = 8;
        } else
            wwindow = p->vld.window;
        newbit = (wwindow >> 7) & 1;
        p->vld.window = wwindow << 1;
        p->vld.bit_count--;
        result = (result << 1) | newbit;
    }
    *(res)= result;
    return 0;
}

/*-----------------------------------*/
/* extract a single symbol from file */
/* using specified huffman table ... */
/*-----------------------------------*/
int get_symbol2(Processframe_State *p, int select,
                unsigned char *res) {
    unsigned char temp;
    long code = 0;
    int length;
    int index;

    for (length = 0; length < 16; length++) {
        get_one_bit2(p, &temp);

        code = (2 * code) | temp;
        if (code <= p->vld.MaxCode[select][length])
            break;
    }
    index = p->vld.ValPtr[select][length] + code -
        p->vld.MinCode[select][length];
    if (index < MAX_SIZE(select / 2)) {
        *(res)=p->vld.HTable[select][index];
        return 0;
    }
#ifndef CASS
    printf("\tWARNING:\tOverflowing symbol table !\n");
#endif
    return 1;
}

int get_one_bit2(Processframe_State *p, unsigned char *res) {
    int newbit;
    unsigned char aux, wwindow;

    *(res) = 0;
    if (p->vld.bit_count == 0) {
        my_read(&wwindow, sizeof(wwindow), p);
        if (wwindow == 0xFF) {
            my_read(&aux, sizeof(aux), p);
            p->vld.bit_count = 0;
        }
        p->vld.bit_count = 8;
    } else
        wwindow = p->vld.window;

    newbit = (wwindow >> 7) & 1;
    p->vld.window = wwindow << 1;
    p->vld.bit_count--;
    *(res) = newbit;
    return 0;
}



//------------------------------------------------------------------------
/* utility and counter to return the number of bits from file */
/* right aligned, masked, first bit towards MSB's               */

inline unsigned int get_size2(Processframe_State *p) {
    unsigned char aux[2];
    my_read(&aux[0], 1*(sizeof(aux[0])), p);
    my_read(&aux[1], 1*(sizeof(aux[1])), p);
    return ((aux[0] << 8) | aux[1]);  /* big endian */
}

//skip a segment we don't want
void skip_segment2(Processframe_State *p)
{
    unsigned int size;
    unsigned char tag[5], waste;
    unsigned int i;

    size = get_size2(p);
    if (size > 5) {
        for (i = 0; i < 4; i++)
            my_read(&tag[i], 1*(sizeof(tag[i])), p);
        tag[4] = 0;
        size -= 4;
    }
    for(i=0; i<(size - 2); i++)
        my_read(&waste, 1*(sizeof(waste)), p);
}

/*----------------------------------------------------------------*/
/* find next marker of any type, returns it, positions just after */
/* EOF instead of marker if end of file met while searching ...   */
/*----------------------------------------------------------------*/
unsigned int get_next_MK2(Processframe_State *p) {
    unsigned char bufp;
    unsigned int c;
    int ffmet = 0;
    int locpassed = -1;

    do {
        my_read(&bufp, sizeof(bufp), p);
        c = (unsigned int)bufp;
        switch (c) {
        case 0xFF:
            ffmet = 1;
            break;
        case 0x00:
            ffmet = 0;
            break;
        default:
            if (ffmet){
                return (0xFF00 | c);
            }
            ffmet = 0;
            break;
        }
        locpassed++;
    } while (c!= (unsigned int)EOF);
    return (unsigned int)EOF;
}

/*----------------------------------------------------------*/
/* Loading of Huffman table, with leaves drop ability       */
/*----------------------------------------------------------*/
int load_huff_tables(Processframe_State *p) {
    unsigned char aux, buf, waste;
    int size, Mclass, id, max;
    int LeavesN, LeavesT, i;
    int AuxCode;

    size = get_size2(p);/* this is the tables' size */

    size -= 2;
    while ((size > 0))  {
        my_read(&aux, 1*(sizeof(aux)), p);

        Mclass = first_quad(aux);  /* AC or DC */
        id = second_quad(aux);    /* table no */

        id = HUFF_ID(Mclass, id);

        size--;
        LeavesT = 0;
        AuxCode = 0;
        for (i = 0; i < 16; i++) {
            my_read(&buf, 1*(sizeof(buf)), p);

            LeavesN = buf;
            p->vld.ValPtr[id][i] = LeavesT;
            p->vld.MinCode[id][i] = AuxCode * 2;
            AuxCode = p->vld.MinCode[id][i] + LeavesN;
            p->vld.MaxCode[id][i] = (LeavesN) ? (AuxCode - 1) : (-1);
            LeavesT += LeavesN;
        }

        size -= 16;
        if (LeavesT > MAX_SIZE(Mclass)) {
            max = MAX_SIZE(Mclass);
            printf("\tWARNING:\tTruncating Table by %d symbols\n",
                   LeavesT - max);
        } else
            max = LeavesT;

        for (i = 0; i < max; i++) { /* get huffman table */
            my_read(&buf, 1*(sizeof(buf)), p);
            p->vld.HTable[id][i] = buf;  /* load in raw order */
        }

        for (i = max; i < LeavesT; i++) {
            my_read(&waste, 1*(sizeof(waste)), p);
        }
        size -= LeavesT;
    }
    return 0;
}


/**
 * unzigzag block
 */
int* unZigZag(int dest[64], int src[64]) {
  int i;
  for (i = 0; i < 64; i++) {
    dest[ZIGZAG_COEFFS[i]] = src[i] ;
  }
  return dest;
}

/**
 * unquantify block
 */
int* unquantify(int block[64], unsigned char qtable[64]) {
  int i;
  for (i = 0; i < 64; i++) {
      block[i] = block[i] * qtable[i];
  }
  return block;
}

/**
 *
 */
int intceil(int N, int D) {
   int i = N / D;
   if (N > D * i) i++;
   return i;
}

/**
 *
 */
int intfloor(int N, int D) {
   int i = N / D;
   if (N < D * i) i--;
   return i;
}

/**
 * transform JPEG number format into usual 2's complement format
 */
int reformat(unsigned long s, int good) {
    unsigned int st;
    if (!good)
        return 0;

    st = 1 << (good - 1); //2^(good - 1)
    if (s < st) {
        return (s + 1 + ((-1) << good));
    } else {
        return s;
    }
}

/**
 *
 */
inline int DESCALE(int x, int n) {
    return (x + (1 << (n - 1)) - (x < 0)) >> n;
}

/**
 *
 */
inline int ADD(int x, int y) {
  int mini = 0;
  int maxi = 0;
  int r = x + y;

  if (r > maxi)    maxi = r;
  if (r < mini)    mini = r;
  return r; //in effect: &0x0000FFFF
}

/**
 *
 */
inline int SUB(int x, int y) {
  int mini = 0;
  int maxi = 0;
  int r = x - y;

  if (r > maxi) maxi = r;
  if (r < mini) mini = r;
  return r; //in effect: &0x0000FFFF
}

/**
 *
 */
inline int CMUL(int c, int x) {
  int mini = 0;
  int maxi = 0;
  int r = c * x;
  // less accurate rounding here also works fine
  r = (r + (1 << (C_BITS - 1))) >> C_BITS;
  if (r > maxi)    maxi = r;
  if (r < mini)    mini = r;
  return r;
}

/**
 * rotate (x,y) over angle k * pi / 16 (counter-clockwise) and scale with f
 */
inline void rot(int f, int k, int x, int y, int *rx, int *ry) {
  int COS[2][8] = {
      {c0_1, c1_1, c2_1, c3_1, c4_1, c5_1, c6_1, c7_1},
      {c0_s2, c1_s2, c2_s2, c3_s2, c4_s2, c5_s2, c6_s2, c7_s2}
  };
#define Cos(k) COS[f][k]
#define Sin(k) Cos(8-k)
  *rx = SUB(CMUL(Cos(k), x), CMUL(Sin(k), y));
  *ry = ADD(CMUL(Sin(k), x), CMUL(Cos(k), y));
#undef Cos
#undef Sin
}


/**
 * inverse 1-D discrete cosine transform. The result Y is scaled
 * up by factor sqrt(8). original Loeffler algorithm
 */
inline void idct_1d(int *Y) {
    int z1[8], z2[8], z3[8];

    //stage 1
    but(Y[0], Y[4], z1[1], z1[0]);
    rot(1, 6, Y[2], Y[6], &z1[2], &z1[3]);
    but(Y[1], Y[7], z1[4], z1[7]);
    z1[5] = CMUL(sqrt2, Y[3]);
    z1[6] = CMUL(sqrt2, Y[5]);

    //stage 2
    but(z1[0], z1[3], z2[3], z2[0]);
    but(z1[1], z1[2], z2[2], z2[1]);
    but(z1[4], z1[6], z2[6], z2[4]);
    but(z1[7], z1[5], z2[5], z2[7]);

    //stage 3
    z3[0] = z2[0];
    z3[1] = z2[1];
    z3[2] = z2[2];
    z3[3] = z2[3];
    rot(0, 3, z2[4], z2[7], &z3[4], &z3[7]);
    rot(0, 1, z2[5], z2[6], &z3[5], &z3[6]);

    //final stage 4
    but(z3[0], z3[7], Y[7], Y[0]);
    but(z3[1], z3[6], Y[6], Y[1]);
    but(z3[2], z3[5], Y[5], Y[2]);
    but(z3[3], z3[4], Y[4], Y[3]);
}

/**
 * inverse 2-D discrete cosine transform
 */
void IDCT(int input[8][8], unsigned char output[8][8]) {
#define Y(i,j) Y[8*i+j]
#define X(i,j) (output[i][j])
    int Y[64];
    int k, l;
    //int mini = INT_MAX2;
    //int maxi = INT_MIN2;

    for (k = 0; k < 8; k++) { //pass 1: process rows.
        for (l = 0; l < 8; l++) { //prescale k-th row:
            Y(k, l) = SCALE(input[k][l], S_BITS);
        }
        idct_1d(&Y(k, 0)); //1-D IDCT on k-th row
        //result Y is scaled up by factor sqrt(8) * 2^S_BITS.
    }
    for (l = 0; l < 8; l++) { //pass 2: process columns.
      int Yc[8];

      for (k = 0; k < 8; k++) {
          Yc[k] = Y(k, l);
      }
      idct_1d(Yc); //1-D IDCT on l-th column
      for (k = 0; k < 8; k++) { //result is once more scaled up by a factor sqrt(8)
          int r = 128 + DESCALE(Yc[k], S_BITS + 3);
          r = r > 0 ? (r < 255 ? r : 255) : 0; //clip to 8 bits unsigned
          X(k, l) = r;
      }
    }
#undef X
#undef Y
}
