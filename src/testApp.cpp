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
	zeroCounter = 0;
	//ofLogToFile("log.log", false);
#if defined(__APPLE__) || defined(__NO_KINECT__)
    noKinectImage.loadImage("images/noKinect.jpg");
    colorImage.allocate(320, 240);
    grayImage.allocate(320, 240);
    colorImage.setFromPixels(noKinectImage.getPixels(), 320, 240);
    grayImage = colorImage;
    noKinectImage.clear();
#endif
	//Filter Flags
	simpleROISmooth = false;
	frameMerge = false;
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
	gui2 = new ofxUICanvas(WIDTH - CONTROL_WIDTH, BANNER_HEIGHT, CONTROL_WIDTH, CONTROL_HEIGHT);
    gui->setWidgetSpacing(8);
    gui2->setWidgetSpacing(8);
	gui->addWidgetDown(new ofxUILabel("Kinect w/ openframeworks by Siyuan Gao", OFX_UI_FONT_MEDIUM));
	gui->addWidgetDown(new ofxUILabel("Tohoku University, Purdue University", OFX_UI_FONT_SMALL));
    gui2->addWidgetDown(new ofxUILabel("Noise Reduction", OFX_UI_FONT_MEDIUM));
	gui2->addSpacer();
	gui2->addWidgetDown(new ofxUIToggle("Enable Frame Merge", frameMerge, 20, 20));
    gui2->addWidgetDown(new ofxUISlider(150, 20, 1, 5, &innerBand, "Inner Band Size"));
    gui2->addWidgetDown(new ofxUISlider(150, 20, 1, 5, &outerBand, "Outer Band Size"));
    gui2->addWidgetDown(new ofxUIRotarySlider(40, 0, 1, &innerWeight, "Inner Band Weight"));
	gui2->addWidgetDown(new ofxUILabelButton(150,false,"Capture"));
	ofAddListener(gui2->newGUIEvent, this, &testApp::guiEvent);
	gui->loadSettings("GUI/guiSettings_1.xml");
	gui2->loadSettings("GUI/guiSettings_2.xml");
}

//--------------------------------------------------------------
void testApp::update(){
#ifndef __APPLE__
#ifndef __NO_KINECT__
        kinect.update();
        if(kinect.isFrameNew()) {
                grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
                colorImage.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);
        }
		if(frameMerge)
			frameMergeFilter();
#endif
#endif
}

void testApp::frameMergeFilter(){
}

void testApp::simpleROI()
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
}

//--------------------------------------------------------------
void testApp::draw(){
	colorImage.draw(LEFT_SPACING, BANNER_HEIGHT + TOP_SPACING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	grayImage.draw(LEFT_SPACING, BANNER_HEIGHT + DISPLAY_HEIGHT + TOP_SPACING * 2, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	ofDrawBitmapStringHighlight("x: " + ofToString(mouseX) + " y: " + ofToString(mouseY), 20, 680, ofColor::seaGreen, ofColor::white);
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
	if(widgetName == "Capture"){
		if(((ofxUILabelButton*) e.widget)->getValue()) {
#ifndef __APPLE__
#ifndef __NO_KINECT__
			ofLogNotice() << "Capturing Depth Image";
			ofSaveImage(kinect.getPixelsRef(), "capture/" + ofGetTimestampString() + ".jpg", OF_IMAGE_QUALITY_BEST);
			c.saveToRaw("capture/" + ofGetTimestampString() + ".raw", kinect.getRawDepthPixels());
			c.saveToCompressedPng("capture/" + ofGetTimestampString() + ".png", kinect.getRawDepthPixels());
#endif
#endif
		}
	}
}

