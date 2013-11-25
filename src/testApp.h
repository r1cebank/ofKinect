#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxDepthImageCompressor.h"
#include "FrameMerger.h"
#include "Filter.h"

#define WIDTH 1200
#define HEIGHT 700
#define CONTROL_WIDTH 210
#define CONTROL_HEIGHT 600
#define BANNER_HEIGHT 50
#define DISPLAY_HEIGHT 480
#define DISPLAY_WIDTH 640

#define LEFT_SPACING 20
#define TOP_SPACING 20

//#define __NO_KINECT__

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//Util Functions
		void translateCoord(int &x, int &y, int widgetX, int widgetY);
        void initData();

		//Filter Functions
		void simpleROI();
		void frameMergeFilter();
		void sharpenImage();
		void invertImage();
		void findContour();
		void drawContours();

		//Filter Var
		FrameMerger *merger;
		Filter *filter;

		//UI Functions
        void initUI();
		void exit();
		void guiEvent(ofxUIEventArgs &e);

		//UI Canvas
		ofxUICanvas *gui;
		ofxUICanvas *gui2;

		//UI Widgets
		ofxUIToggle *sharpen, *showContour, *findHolesToggle, *useApproxToggle, *useInvert;
		ofxUIIntSlider *sharpenBlurSlider, *boundSlider, *minAreaSlider, *maxAreaSlider, *nConsideredSlider;
		int sharpenBlurSize;
		bool sharpenOn, invertOn, contourOn;
		int minArea, maxArea, nConsidered;
		bool findHoles, useApprox;
		
		//Sensor
		ofxKinect kinect;
        void initKinect();

		//OpenCV
		ofxCvColorImage colorImage;
		ofxCvGrayscaleImage grayImage; // grayscale depth image
		ofxCvColorImage distanceImage;
		ofxCvGrayscaleImage invertBackup, sharpenBackup;
		ofxCvContourFinder contourFinder;
	
		// used for viewing the point cloud
		ofEasyCam easyCam;

		//Misc Variables
		ofVec3f distance;
		int zeroCounter;
        float innerWeight;
        float innerBand, outerBand;
        ofImage noKinectImage;
		ofxDepthImageCompressor c;
		bool simpleROISmooth;
		bool frameMerge;
		bool filterOn;
		//UI Items


};
