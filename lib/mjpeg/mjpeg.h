#ifndef __MJPEG_H__
#define __MJPEG_H__

#ifdef VIEWER
#include "CImg.h"
using namespace cimg_library;
#endif

typedef struct _mjpeg_state {
    int frame_number;

    int len;
    unsigned char *ptr;
    unsigned char *hptr;
    unsigned char *fptr;

#ifdef VIEWER
    CImgDisplay *main_disp;
#endif
} Mjpeg_State;

// Main methods
struct _mjpeg_state *mjpeg_init(); 
int mjpeg_fire(struct _mjpeg_state *state); 
void mjpeg_finish(struct _mjpeg_state *state); 

#endif