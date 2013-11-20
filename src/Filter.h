#pragma once
#include "ofxOpenCv.h"

class Filter
{
public:
	Filter(void);
	Filter(int);
	~Filter(void);
	unsigned char* getFrame(ofxCvGrayscaleImage);
	unsigned char getAverageFromImage(unsigned char*, int);
	int boundSize;
	unsigned char averageSelected;
private:
	ofxCvGrayscaleImage filteredImage;
};

