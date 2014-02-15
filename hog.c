#include "hog.h"

void calculateGradientVectors(Image* img) {
    int height = img->h;
    int width = img->w;
    unsigned char* data = img->data;
    GradVec* vectors = img->vectors;
    
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
}

int getBin(float angle) {
    if (angle < 0) {
        angle += M_PI_2;
    }
    return floor(angle * kBins / M_PI);
}

void getHistogram(Histogram* histogram, GradVec* vectors, const int x, const int y) {
    for (int i = 0; i < kCellSize; i++) {
        for (int j = 0; j < kCellSize; j++) {
            GradVec* v = &vectors[y*kCellSize + i + x + j];
            int bin = getBin(v->orient);
            histogram->h[bin] += v->mag;
        }
    }
}

void extractHistograms(Image* img) {
    memset(img->histograms, 0, img->cellsHigh * img->cellsWide * sizeof(Histogram));
    calculateGradientVectors(img);
    for (int i = 0; i < img->cellsHigh; i++) {
        for (int j = 0; j < img->cellsWide; j++) {
            Histogram* h = &img->histograms[i*img->cellsWide + j];
            getHistogram(h, img->vectors, j * kCellSize, i * kCellSize);
        }
    }
}

float getBlockDenominator(Image* img, const int xCell, const int yCell) {
    const float epsilon = 0.00001;
    float sum = 0;
    for(int y = yCell; y < xCell + kBlockSize; y++) {
        for(int x = xCell; x < xCell + kBlockSize; x++) {
            Histogram *h = &img->histograms[y*img->cellsWide + x];
            for (int i = 0; i < kBins; i++) {
                sum += pow(h->h[i], 2);
            }
        }
    }
    float denom = sqrt(sum + epsilon);
    return denom;
}

int setBlockHOG(Image* img, int pos, const int xCell, const int yCell, const float denominator) {
    for(int y = yCell; y < yCell + kBlockSize; y++) {
        for(int x = xCell; x < xCell + kBlockSize; x++) {
            Histogram *h = &img->histograms[y*img->cellsWide + x];
            for (int i = 0; i < kBins; i++) {
                printf("pos = %d\n", pos );
                img->hog.value[pos] = h->h[i] / denominator;
                pos++;
            }
        }
    }
    return pos;
}

HOG* getHOG(Image* img, const int xCell, const int yCell) {
    int pos = 0;
    for(int y = yCell; y < yCell + kHOGH; y++) {
        for(int x = xCell; x < xCell + kHOGW; x++) {
            float denominator = getBlockDenominator(img, x, y);
            pos = setBlockHOG(img, pos, x, y, denominator);
            printf("fuck pos = %d\n", pos );
        }
    }
    return &img->hog;
}

Image* createImage(const int w, const int h) {
    Image* img = malloc(sizeof(Image));
    img->w = w;
    img->h = h;
    img->data = malloc(w * h * sizeof(unsigned char));
    img->vectors = malloc(w * h * sizeof(GradVec));
    img->cellsHigh = floor(h / kCellSize);
    img->cellsWide = floor(w / kCellSize);
    img->histograms = malloc(img->cellsHigh * img->cellsWide * sizeof(Histogram));
    img->hog.value = malloc(kHOGLength * sizeof(float));
    return img;
}

void freeImage(Image* img) {
    free(img->data);
    free(img->vectors);
    free(img->histograms);
    free(img->hog.value);
    free(img);
}

void testCase() {
    Image* img = createImage(300, 300);
    extractHistograms(img);
    /*for (int y = 0; y < img->cellsHigh; y++) {
        for (int x=0; x < img->cellsWide; x++) {
            getHOG(img, x, y);
        }
    }
    */
    printf("point 1\n");
    getHOG(img, 2, 2);
    printf("point 2\n");
    freeImage(img);
    printf("point 3\n");
    
}

