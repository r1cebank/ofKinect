#include "Filter.h"


Filter::Filter(void)
{
	Filter(1);
}

Filter::Filter(int bound)
{
	boundSize = bound;
}

unsigned char* Filter::getFrame(ofxCvGrayscaleImage image)
{
	filteredImage.setFromPixels(image.getPixelsRef());
    unsigned char *original = image.getPixels();
    unsigned char *filtered = filteredImage.getPixels();
    int filterX = 0, filterY = 0;
	long long start = ofGetElapsedTimeMillis();
    for(int i = 0; i < 640*480; i++){
        if(original[i] == 0){
            //Set ROI, get roi image, get average and update
            //filtered[i] = 0;
            filterY = i / 640;
            filterX = i % 640;
            filteredImage.resetROI();
            filteredImage.setROI(filterX - boundSize, filterY - boundSize, boundSize*2+1, boundSize*2+1);
			averageSelected = getAverageFromImage(filteredImage.getRoiPixels(), pow((double)((boundSize * 2)+ 1), 2));
			filtered[i] = averageSelected;
        }
            //Find the zeros in image and replace with the average.
    }
	long long end = ofGetElapsedTimeMillis();
	ofLogNotice() << "Elapsed time: " << end-start << "ms";
	return filteredImage.getPixels();
}

unsigned char Filter::getAverageFromImage(unsigned char* input, int size){
	int totalValue = 0, nonZero = 0;
	for(int i = 0; i < size; i++){
		if(input[i] != 0){
				totalValue += input[i];
				nonZero++;
		}
	}
	if(nonZero != 0)
		return (unsigned char)(totalValue / nonZero);
	else
		return 0;
}

Filter::~Filter(void)
{
}
