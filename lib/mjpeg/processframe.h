#ifndef PROCESSFRAME_H
#define PROCESSFRAME_H

#include "jpeg.h"
#include "mjpeg.h"

typedef struct _vld_state {
    int mx_size;
    int my_size; //picture size in units of MCUs
    int x_size;
    int y_size; //picture size in pixels
    unsigned char DC_Table0[MAX_SIZE(DC_CLASS)];
    unsigned char DC_Table1[MAX_SIZE(DC_CLASS)];
    unsigned char AC_Table0[MAX_SIZE(AC_CLASS)];
    unsigned char AC_Table1[MAX_SIZE(AC_CLASS)];
    unsigned char *HTable[4];
    int MinCode[4][16];
    int MaxCode[4][16];
    int ValPtr[4][16];
    unsigned char window;
    unsigned char bit_count; //available bits in the window
    unsigned char QTable[64];
} VLD_State;

//local variables
typedef struct _local_states {
    int fLen;                      //frame size
    char *fptr;           //current read ptr
    VLD_State vld;                 //VLD state
    int num_iter;                  //number of iteration
    unsigned char LineBuffer[MAX_WIDTH * MCU_sx];
} Processframe_State;

unsigned char *decodeFrame(struct _mjpeg_state *state, int fLen, unsigned char *frame, int *x_size, int *y_size); 

#endif
