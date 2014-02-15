#include "HOG.h"

Vec* gradientVectors(Image* img) {
    int height = img->h;
    int width = img->w;
    unsigned char* data = img->data;
    
    Vec* vectors = (Vec *)malloc(height * width * sizeof(Vec));
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int c = y*width + x;
            int prevX = (x == 0 ? 0 : data[c - 1]);
            int nextX = (x == width - 1 ? 0 : data[c + 1]);
            int prevY = (y == 0 ? 0 : data[c - width]);
            int nextY = (y == height - 1 ? 0 : data[c + width]);
            
            // kernel [-1, 0, 1]
            int gradX = -prevX + nextX;
            int gradY = -prevY + nextY;
            
            vectors[c].mag = sqrt(gradX*gradX + gradY*gradY);
            vectors[c].orient = atan2(gradY, gradX);
        }
    }
    return vectors;
}

int binFor(float radians) {
    float angle = radians * (180 / M_PI);
    if (angle < 0) {
        angle += 180;
    }
    
    // center the first bin around 0
    angle += 90 / kBins;
    //angle %= 180;
    if (angle > 180) {
        angle -= 180;
    }
    
    int bin = floor(angle / 180 * kBins);
    return bin;
}

void getHistogram(Histogram* histogram, Vec* vectors, int x, int y) {
    for (int i = 0; i < kCellSize; i++) {
        for (int j = 0; j < kCellSize; j++) {
            Vec v = vectors[y*kCellSize + i + x + j];
            int bin = binFor(v.orient);
            histogram->h[bin] += v.mag;
        }
    }
}

Histogram* extractHistograms(Image* img, Histogram* histograms, int cellsHigh, int cellsWide) {
    Vec* v = gradientVectors(img);
    for (int i = 0; i < cellsHigh; i++) {
        for (int j = 0; j < cellsWide; j++) {
            Histogram* h = &histograms[i*cellsWide + j];
            getHistogram(h, v, j * kCellSize, i * kCellSize);
        }
    }
    return histograms;
}


void normalize(vector, int length) {
    const float epsilon = 0.00001;
    float sum = 0;
    for (int i = 0; i < length; i++) {
        sum += pow(vector[i], 2);
    }
    float denom = sqrt(sum + epsilon);
    for (int i = 0; i < length; i++) {
        vector[i] /= denom;
    }
}


void getBlock(Block* square, matrix, x, y, length) {
    //var square = [];
    
    int c = 0;
    for (int i = y; i < y + length; i++) {
        for (int j = x; j < x + length; j++) {
            //square.push(matrix[i][j]);
            square[c] = matrix[i*length + j];
            c++;
        }
    }
    //return Array.prototype.concat.apply([], square);
}

void extractHOGFromHistograms(HOG* hog, Histogram* histograms, int blocksHigh, int blocksWide) {
    
    //var blocks = [];
    
     int c = 0;
     for (int y = 0; y < blocksHigh; y += kBlockStride) {
        for (int x = 0; x < blocksWide; x += kBlockStride) {
            getBlock(&hog->block[c], histograms, x, y, kBlockSize);
            normalize(block);
            c++;
        }
    }
    //return Array.prototype.concat.apply([], blocks);
}

HOG* extractHOG(Image* img) {
    int cellsHigh = floor(img->h / kCellSize);
    int cellsWide = floor(img->w / kCellSize);
    int histlen = cellsHigh * cellsWide * sizeof(Histogram);
    Histogram* histograms = malloc(histlen);
    memset(histograms, 0, histlen);
    extractHistograms(img, histograms, cellsHigh, cellsWide);
//
    int blocksHigh = cellsHigh - kBlockSize + 1;
    int blocksWide = cellsWide - kBlockSize + 1;
    HOG* hog = malloc(?*sizeof(?));
    extractHOGFromHistograms(hog, histograms, blocksHigh, blocksWide);
    return hog;
}


