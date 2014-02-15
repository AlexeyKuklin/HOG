#ifndef HOG_HOG_h
#define HOG_HOG_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Image.h"

#define kCellSize     4  // length of cell in px
#define kBlockSize    2  // length of block in number of cells
#define kBlockStride  1  // number of cells to slide block window by (block overlap)
#define kBins         6  // bins per histogram


typedef struct {
    int length;
    float *value;
} HOG;


typedef struct {
    float mag;
    float orient;
} Vec;


typedef struct {
    float h[kBins];
} Histogram;


#endif
