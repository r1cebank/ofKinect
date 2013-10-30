#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	//ofLogToFile("log.log", false);
	ofSetBackgroundColor(153, 151, 136);
	gui = new ofxUICanvas(0,0, WIDTH+CONTROL_WIDTH, BANNER_HEIGHT);
	gui2 = new ofxUICanvas(WIDTH, BANNER_HEIGHT, CONTROL_WIDTH, CONTROL_HEIGHT);
	gui->addWidgetDown(new ofxUILabel("Kinect w/ openframeworks by Siyuan Gao", OFX_UI_FONT_MEDIUM));
	gui->addWidgetDown(new ofxUILabel("Tohoku University, Purdue University", OFX_UI_FONT_SMALL));
	gui2->addWidgetDown(new ofxUILabel("Control", OFX_UI_FONT_MEDIUM));
	ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent); 
	gui->loadSettings("GUI/guiSettings_1.xml"); 
	gui2->loadSettings("GUI/guiSettings_2.xml"); 

	/* Finish with UI Kinect setting up*/
#ifndef __APPLE__
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
	colorImg.allocate(kinect.width, kinect.height);
#endif
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void testApp::update(){
	kinect.update();
}

//--------------------------------------------------------------
void testApp::draw(){
#ifndef __APPLE__
	kinect.drawDepth(20, BANNER_HEIGHT + 10, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	kinect.draw(20, BANNER_HEIGHT + DISPLAY_HEIGHT + 20, DISPLAY_WIDTH, DISPLAY_HEIGHT);
#endif
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
    delete gui; 
	delete gui2;
}

void testApp::guiEvent(ofxUIEventArgs &e)
{
	string widgetName = e.widget->getName();
}

