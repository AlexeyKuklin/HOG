#ifndef HOG_HOG_h
#define HOG_HOG_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define kCellSize      8  // length of cell in px
#define kBlockSize     2  // length of block in number of cells
#define kBlockStride   1  // number of cells to slide block window by (block overlap)
#define kBins          9  // bins per histogram
#define kHOGCellWidth  7  // width of descriptor in number of cells
#define kHOGCellHeight 7  // height of descriptor in number of cells
#define kHOGLength     kBlockSize*kBlockSize*kBins*(kHOGCellWidth - 1)*(kHOGCellHeight - 1)  // total length of descriptor


typedef struct {
    float* value;
} HOG;


typedef struct {
    float mag;
    unsigned char bin;
} GradVec;


typedef struct {
    float h[kBins];
} Histogram;


typedef struct {
    int h;
    int w;
    unsigned char* data;
    GradVec* vectors;
    int cellsHigh;
    int cellsWide;
    Histogram* histograms;
    HOG hog;
} Image;

Image* createImage(const int w, const int h);
void freeImage(Image* img);

void testCase();

#endif
