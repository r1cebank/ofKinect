#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	smoothPixel = new unsigned char[kinect.width*kinect.height];
	//ofLogToFile("log.log", false);
	ofSetBackgroundColor(153, 151, 136);
	//TODO: Move UI Setting in another function
	gui = new ofxUICanvas(0,0, WIDTH+CONTROL_WIDTH, BANNER_HEIGHT);
	gui2 = new ofxUICanvas(WIDTH, BANNER_HEIGHT, CONTROL_WIDTH, CONTROL_HEIGHT);
	gui->addWidgetDown(new ofxUILabel("Kinect w/ openframeworks by Siyuan Gao", OFX_UI_FONT_MEDIUM));
	gui->addWidgetDown(new ofxUILabel("Tohoku University, Purdue University", OFX_UI_FONT_SMALL));
	gui2->addWidgetDown(new ofxUILabel("Values", OFX_UI_FONT_MEDIUM));
	gui2->addSpacer();
	gui2->addWidgetDown(new ofxUILabel("Mouse X", "", OFX_UI_FONT_SMALL));
	gui2->addWidgetDown(new ofxUILabel("Mouse Y", "", OFX_UI_FONT_SMALL));
	gui2->addWidgetDown(new ofxUILabel("Distance", "", OFX_UI_FONT_SMALL));
	gui2->addWidgetDown(new ofxUILabel("Zero", "", OFX_UI_FONT_SMALL));
	gui2->addWidgetDown(new ofxUILabel("Control", OFX_UI_FONT_MEDIUM));
	gui2->addSpacer();
	gui2->addWidgetDown(new ofxUIToggle(20, 20, false, "Translate Mouse Coord"));
	gui2->addWidgetDown(new ofxUISlider(100, 20, 0, 20, &gaussianValue, "Blur"));
	ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent); 
	gui->loadSettings("GUI/guiSettings_1.xml"); 
	gui2->loadSettings("GUI/guiSettings_2.xml"); 

	/* Finish with UI Kinect setting up*/
#ifndef __APPLE__
#ifndef __NO_KINECT__
	kinect.setRegistration(true);
	kinect.init();
	kinect.open();
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
		ofLogNotice() << "sensor width: " << kinect.width;
		ofLogNotice() << "sensor height: " << kinect.height;
	}
	colorImage.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);

#endif
#endif
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void testApp::update(){
#ifndef __APPLE__
#ifndef __NO_KINECT__
	kinect.update();
	if(kinect.isFrameNew()) {
		grayImage.setFromPixels(smooth(kinect.getDepthPixels()), kinect.width, kinect.height);
		colorImage.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);
		grayImage.blurGaussian(floor(gaussianValue));
	}
#endif
#endif
	if((mouseX > 20 && mouseY > BANNER_HEIGHT + DISPLAY_HEIGHT + 20) &&
		(mouseX < 20 + DISPLAY_WIDTH && mouseY < BANNER_HEIGHT + DISPLAY_HEIGHT * 2 + 20)) {
			currentX = mouseX;
			currentY = mouseY;
			translateCoord(currentX, currentY, 20, BANNER_HEIGHT + DISPLAY_HEIGHT + 20);
#ifndef __APPLE__
#ifndef __NO_KINECT__
			if(kinect.getDistanceAt(currentX*2, currentY*2) > 0) {
				distance = kinect.getWorldCoordinateAt(currentX*2, currentY*2);
			}
#endif
#endif
	}
}

unsigned char * testApp::smooth(unsigned char* input)
{
	zeroCounter = 0;
#ifndef __APPLE__
#ifndef __NO_KINECT__
	for(int i = 0; i< kinect.height* kinect.width; i++) {
		if(input[i] == 0) {
			zeroCounter++;
		}
	}
#endif
#endif
	return input;
}

void testApp::translateCoord(int &x, int &y, int widgetX, int widgetY) {
	y = y - widgetY;
	x = x - widgetX;
}

//--------------------------------------------------------------
void testApp::draw(){
	//kinect.drawDepth(20, BANNER_HEIGHT + 10, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	//kinect.draw(20, BANNER_HEIGHT + DISPLAY_HEIGHT + 20, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	colorImage.draw(20, BANNER_HEIGHT + 10, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	grayImage.draw(20, BANNER_HEIGHT + DISPLAY_HEIGHT + 20, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	//Draw depth and color image
	((ofxUILabel*)(gui2->getWidget("Distance")))->setLabel("Distance: " + ofToString(distance));
	((ofxUILabel*)(gui2->getWidget("Zero")))->setLabel("Zero Values: " + ofToString(zeroCounter));
	if(((ofxUIToggle*)gui2->getWidget("Translate Mouse Coord"))->getValue()) {
		((ofxUILabel*)(gui2->getWidget("Mouse X")))->setLabel("Mouse X: " + ofToString(currentX));
		((ofxUILabel*)(gui2->getWidget("Mouse Y")))->setLabel("Mouse Y: " + ofToString(currentY));
	}
	else {
		((ofxUILabel*)(gui2->getWidget("Mouse X")))->setLabel("Mouse X: " + ofToString(mouseX));
		((ofxUILabel*)(gui2->getWidget("Mouse Y")))->setLabel("Mouse Y: " + ofToString(mouseY));
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::exit()
{
	gui->saveSettings("GUI/guiSettings_1.xml");
	gui2->saveSettings("GUI/guiSettings_2.xml");
#ifndef __APPLE__
#ifndef __NO_KINECT__
	kinect.close();
#endif
#endif
    delete gui; 
	delete gui2;
}

void testApp::guiEvent(ofxUIEventArgs &e)
{
	string widgetName = e.widget->getName();
}

