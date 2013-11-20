#include "FrameMerger.h"


FrameMerger::FrameMerger(int size, int arraySize){
	maxSize = size;
	outputImage = new unsigned char [arraySize];
	imageSize = arraySize;
	currentSize = 0;
}

FrameMerger::FrameMerger(int arraySize){
	FrameMerger(2, arraySize);
}

FrameMerger::~FrameMerger(void){
}

unsigned char* FrameMerger::addFrame(unsigned char* image){
	unsigned char* temp = new unsigned char [imageSize];
	memcpy(temp, image, imageSize);
	frames.push_back(temp);
	outputImage = frames[frames.size() - 1];
	if(frames.size() > maxSize) {
		frames.erase(frames.begin());
	}
	for(int i = 0; i < imageSize; i++){
		if(outputImage[i] == 0) {
			for (int it = 0; it< frames.size(); it++) {
				if(frames[it][i] != 0)
					outputImage[i] = (unsigned char)(frames[it][i]);
					break;
			}
		}
	}
	//ofLogNotice() << "Queue Size: " << frames.size();
	return outputImage;
}

void FrameMerger::reSize(int size){
	maxSize = size;
	//Re-align current data
}