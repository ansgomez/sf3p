#include "mjpeg.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "jpeg.h"
#include "fileString.h" //read video from string
#include "processframe.h"

void readFrame(struct _mjpeg_state *state, int *fLen, unsigned char **buffer) {
    int len = sizeof(_STR);

    if (state->len == 0) {
		state->len = sizeof(_STR);
        state->hptr = _STR;
        state->fptr = state->hptr;
	}

    while (len != 0) {
        state->ptr = (unsigned char *)memchr(state->hptr, 0xFF, state->len);

        if (*(state->ptr + 1) == (unsigned char) '\xD9') { //end of image 0xFFD9

            state->ptr += 2;
            (*fLen) = state->ptr - state->fptr;

            (*buffer) = state->fptr; 

            state->len -= (state->ptr - state->hptr);
            state->hptr = state->ptr;
            state->fptr = state->hptr;

            return;

        } else {
        	state->ptr += 2;
        	state->len -= (state->ptr - state->hptr);
        	state->hptr = state->ptr;
        }

    }
    return;
}

void showVideo(struct _mjpeg_state *state, unsigned int x_size, unsigned int y_size, unsigned char *FrameBuffer){
#ifdef VIEWER
    //create image from display_buffer and display it
    CImg<unsigned char> img(FrameBuffer,
            x_size, y_size),
            visu(x_size, y_size, 1, 1, 0);
    state->main_disp->display(img);
    state->main_disp->paint();
#endif
}

struct _mjpeg_state *mjpeg_init() {
    struct _mjpeg_state *state = (struct _mjpeg_state *) malloc (sizeof (struct _mjpeg_state)); 

    state->frame_number = 0;
    state->len = 0;

    #ifdef VIEWER
    state->main_disp = new CImgDisplay(320, 240, "MJPEG");
    #endif

    return state; 
}

int mjpeg_fire(struct _mjpeg_state *state) {

    int fLen = 0; 
    unsigned char *buffer; 
    readFrame(state, &fLen, &buffer); 

    int x_size, y_size; 
    unsigned char *frame = decodeFrame(state, fLen, buffer, &x_size, &y_size); 
    showVideo(state, x_size, y_size, frame); 

    free(frame); 

    state->frame_number++; 
    return 0; 
}

void mjpeg_finish(struct _mjpeg_state *state){ 

    #ifdef VIEWER
    delete state->main_disp; 
    #endif

    free(state); 
}