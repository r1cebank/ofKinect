#pragma once

#include "ofxOpenCv.h"
#include <vector>

class FrameMerger
{
	public:
		FrameMerger(int size, int arraySize);
		FrameMerger(int arraySize);
		~FrameMerger(void);
		unsigned char* addFrame(unsigned char* image);
		void reSize(int size);

		std::vector<unsigned char*> frames;
		int maxSize;
		int currentSize, imageSize;
		unsigned char *outputImage;
};

