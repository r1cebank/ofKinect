#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    initData();
    ofSetBackgroundColor(230, 230, 230);
    initUI(); //Initialize UI
	/* Finish with UI Kinect setting up*/
#ifndef __APPLE__
#ifndef __NO_KINECT__
    initKinect();
#endif
#endif
	ofSetFrameRate(60);
}

void testApp::initData(){
    ofSetLogLevel(OF_LOG_VERBOSE);
	smoothPixel = new unsigned char[kinect.width*kinect.height];
	//ofLogToFile("log.log", false);
#if defined(__APPLE__) || defined(__NO_KINECT__)
    noKinectImage.loadImage("images/noKinect.jpg");
    colorImage.allocate(320, 240);
    grayImage.allocate(320, 240);
    colorImage.setFromPixels(noKinectImage.getPixels(), 320, 240);
    grayImage.setFromPixels(noKinectImage.getPixels(), 320, 240);
    noKinectImage.clear();
#endif

}

void testApp::initKinect(){
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
}

void testApp::initUI(){
    gui = new ofxUICanvas(0,0, WIDTH+CONTROL_WIDTH, BANNER_HEIGHT);
	gui2 = new ofxUICanvas(WIDTH, BANNER_HEIGHT, CONTROL_WIDTH, CONTROL_HEIGHT);
    gui->setWidgetSpacing(8);
    gui2->setWidgetSpacing(8);
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
	gui2->addWidgetDown(new ofxUIToggle(15, 15, false, "Translate Mouse Coord"));
	gui2->addWidgetDown(new ofxUISlider(150, 20, 0, 20, &gaussianValue, "Blur"));
    gui2->addWidgetDown(new ofxUILabel("Noise Reduction", OFX_UI_FONT_MEDIUM));
	gui2->addSpacer();
    gui2->addWidgetDown(new ofxUISlider(150, 20, 1, 5, &innerBand, "Inner Band Size"));
    gui2->addWidgetDown(new ofxUISlider(150, 20, 1, 5, &outerBand, "Outer Band Size"));
    gui2->addWidgetDown(new ofxUIRotarySlider(40, 0, 1, &innerWeight, "Inner Band Weight"));
	ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
	gui->loadSettings("GUI/guiSettings_1.xml");
	gui2->loadSettings("GUI/guiSettings_2.xml");
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
	if((mouseX > LEFT_SPACING && mouseY > BANNER_HEIGHT + DISPLAY_HEIGHT + TOP_SPACING * 2) &&
		(mouseX < LEFT_SPACING + DISPLAY_WIDTH && mouseY < BANNER_HEIGHT + DISPLAY_HEIGHT * 2 + TOP_SPACING * 2)) {
			currentX = mouseX;
			currentY = mouseY;
			translateCoord(currentX, currentY, LEFT_SPACING, BANNER_HEIGHT + DISPLAY_HEIGHT + TOP_SPACING * 2);
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
	colorImage.draw(LEFT_SPACING, BANNER_HEIGHT + TOP_SPACING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	grayImage.draw(LEFT_SPACING, BANNER_HEIGHT + DISPLAY_HEIGHT + TOP_SPACING * 2, DISPLAY_WIDTH, DISPLAY_HEIGHT);
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

