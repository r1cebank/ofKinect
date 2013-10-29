#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

#define WIDTH 800
#define CONTROL_WIDTH 200
#ifndef CONTROL_HEIGHT
#define CONTROL_HEIGHT 600
#endif
#define HEIGHT 600
#define BANNER_HEIGHT 50
#define DISPLAY_HEIGHT 225
#define DISPLAY_WIDTH 300

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

		//UI Functions
		void exit(); 
		void guiEvent(ofxUIEventArgs &e);

		//UI Canvas
		ofxUICanvas *gui;
		ofxUICanvas *gui2;
		
		//Sensor
		ofxKinect kinect;

		//OpenCV
		ofxCvColorImage colorImg;
		ofxCvGrayscaleImage grayImage; // grayscale depth image
		ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
		ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
		ofxCvContourFinder contourFinder;
	
		// used for viewing the point cloud
		ofEasyCam easyCam;
};
