#include "MA_filter.h"

void init_filter(filter_t* fp)
{
    fp->filled = 0; // initialize buffer
    fp->index = 0;
    fp->sum = 0;
    fp->average = 0;
    for (int i=0; i<D; i++){
        fp->buffer[i] = 0;
    }
}

void filter(filter_t* fp, float acc)
{
    fp->buffer[fp->index] = acc; // new value
    fp->index++; // increment index
    if (fp->index == D) {
        fp->filled = 1; // flag for filled filter
    }
    fp->index = fp->index%D; // wrap-around indexing
    if (fp->filled) { // update output only when filter is filled
        fp->sum = 0;
        for (int i=0; i<D; i++) {
            fp->sum = fp->sum + fp->buffer[i];
        }
        fp->average = fp->sum / D;
    }
}
