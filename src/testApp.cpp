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
	filterOn = false;
	//Filters
	merger = new FrameMerger(3, 640 * 480);
	filter = new Filter(1);
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
	gui2->addWidgetDown(new ofxUIToggle("Enable Frame Merge", &frameMerge, 20, 20));
	gui2->addWidgetDown(new ofxUIToggle("Filter Frame", &filterOn, 20, 20));
	gui2->addWidgetDown(new ofxUILabelButton(150,false,"Capture"));
	gui2->addWidgetDown(new ofxUILabel("Processing", OFX_UI_FONT_MEDIUM));
	gui2->addSpacer();
	sharpen = new ofxUIToggle("Sharpen", &sharpenOn, 15, 15);
	sharpenBlurSlider = new ofxUIIntSlider("Sharpen Blur", 1, 40, &sharpenBlurSize, 170, 20);
	useInvert = new ofxUIToggle("Invert", &invertOn, 15, 15);
	showContour = new ofxUIToggle("Show Contour", &contourOn, 15, 15);
	gui2->addWidgetDown(sharpen);
	gui2->addWidgetDown(sharpenBlurSlider);
	gui2->addWidgetDown(useInvert);
	gui2->addWidgetDown(new ofxUILabel("Contour Finder", OFX_UI_FONT_MEDIUM));
	minAreaSlider = new ofxUIIntSlider("Min Area", 1, 640*480, &minArea, 170, 20);
	maxAreaSlider = new ofxUIIntSlider("Max Area", 1, 640*480, &maxArea, 170, 20);
	nConsideredSlider = new ofxUIIntSlider("nConsidered", 1, 100, &nConsidered, 170, 20);
	findHolesToggle = new ofxUIToggle("Find Holes", &findHoles, 15, 15);
	useApproxToggle = new ofxUIToggle("Use Approximation", &useApprox, 15, 15);
	gui2->addSpacer();
	gui2->addWidgetDown(showContour);
	gui2->addWidgetDown(minAreaSlider);
	gui2->addWidgetDown(maxAreaSlider);
	gui2->addWidgetDown(nConsideredSlider);
	gui2->addWidgetDown(findHolesToggle);
	gui2->addWidgetDown(useApproxToggle);
	gui2->addFPS();
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
		if(frameMerge) {
			frameMergeFilter();
			//ofLogNotice() << "Merging Frame";
		}
		if(filterOn) {
			grayImage.setFromPixels(filter->getFrame(grayImage), kinect.width, kinect.height);
		}
		if(sharpenOn)
			sharpenImage();
		if(invertOn)
			invertImage();
		if(contourOn)
			findContour();
#endif
#endif
}

void testApp::findContour(){
	contourFinder.findContours(grayImage, minArea, maxArea, nConsidered, findHoles, useApprox);
}

void testApp::frameMergeFilter(){
	grayImage.setFromPixels(merger->addFrame(grayImage.getPixels()), kinect.width, kinect.height);
}

void testApp::drawContours(){
	float x, y, area, length, width, height;
	float realX, realY;
	ofPoint centroid;
	string trackingID;
	ofxCvBlob blob;
	double average, centerX, centerY;
	unsigned char maxDev;
	ofNoFill();
	if(contourOn){
		contourFinder.draw(20, 70, 640, 480);
		ofColor c(255, 0, 0);
		for(int i = 0; i < contourFinder.nBlobs; i++) {
			//Setting useful variables
			blob = contourFinder.blobs.at(i);
			ofRectangle r = blob.boundingRect;
			x = r.x; y = r.y; width = r.width; height = r.height; centroid = blob.centroid; area = blob.area;
			length = blob.length; //Messy no return multiline

			//Drawing rectangle
			r.x += 20; r.y += 70;
			c.setHsb(i * 64, 255, 255);
			ofSetColor(c);
			ofRect(r);
			//Drawing point
			ofCircle(centroid.x + 20, centroid.y + 70, 4);
			//Getting Center point and drawing it
			centerX = width / 2 + x;
			centerY = height / 2 + y; //Relative to image start from (0,0)
			ofCircle(centerX + 20, centerY + 70, 2);

			//Setting ROI, getting Image Info
			grayImage.setROI(r.x, r.y, width, height);
			contourImage.setFromPixels(grayImage.getRoiPixelsRef());
			grayImage.resetROI();
			setAvgDev(average, maxDev);

			//Drawing informations
			trackingID = ofToString(average) + "#" + ofToString(area) + "#" + ofToString((int)maxDev) + "#" + 
						ofToString(centroid.x - centerX) + "#" + ofToString(centroid.y - centerY);
			realX = x + 20;
			realY = y + 70;
			ofDrawBitmapString("x:" + ofToString(x) + " y:" + ofToString(y), realX, realY - 11);
			ofDrawBitmapString("width:" + ofToString(width) + " height:" + ofToString(height), realX, realY - 11*2);
			ofDrawBitmapString("area:" + ofToString(area) + " length:" + ofToString(length), realX, realY - 11*3);
			ofDrawBitmapString("tracking ID: " + trackingID, realX, realY - 11*4); //Tracking ID is set from average distance area max/mindev center to centroiddev
			//231#12312#1#2#-1#1
		}
	}
	ofFill();
	ofSetColor(ofColor::white);
}

void testApp::setAvgDev(double &average, unsigned char &maxDev)
{
	int total = 0;
	int positive = 0;
	maxDev = 0;
	unsigned char max = contourImage.getPixels()[0], min = contourImage.getPixels()[0];
	for(int i = 0; i < contourImage.width * contourImage.height; i++)
	{
		if(contourImage.getPixels()[i] != 0)
		{
			total += (int) contourImage.getPixels()[i];
			if(max < contourImage.getPixels()[i])
				max = contourImage.getPixels()[i];
			if(min > contourImage.getPixels()[i])
				min = contourImage.getPixels()[i];
			positive++;
		}
	}
	maxDev = max - min;
	if(positive != 0)
		average = total / positive;
	else
		average = 0;
}

void testApp::simpleROI()
{
	zeroCounter = 0;
#ifndef __APPLE__
#ifndef __NO_KINECT__
	for(int i = 0; i< kinect.height* kinect.width; i++) {
		if(grayImage.getPixels()[i] == 0) {
			zeroCounter++;
		}
	}
#endif
#endif
}

//--------------------------------------------------------------
void testApp::draw(){
	grayImage.draw(LEFT_SPACING, BANNER_HEIGHT + TOP_SPACING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	//colorImage.draw(LEFT_SPACING, BANNER_HEIGHT + TOP_SPACING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	//grayImage.draw(LEFT_SPACING, BANNER_HEIGHT + DISPLAY_HEIGHT + TOP_SPACING * 2, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	ofDrawBitmapStringHighlight("x: " + ofToString(mouseX) + " y: " + ofToString(mouseY), 20, 680, ofColor::seaGreen, ofColor::white);
	if(contourOn)
		drawContours();
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

void testApp::invertImage(){
	grayImage.invert();
}

void testApp::sharpenImage(){
	sharpenBackup.setFromPixels(grayImage.getPixels(), grayImage.width, grayImage.height);
	//ofLogNotice() << "Sharpening...";
	ofxCvGrayscaleImage temp;
	temp.allocate(grayImage.getWidth(), grayImage.getHeight());
	temp.setFromPixels(grayImage.getPixelsRef());
	temp.blurGaussian(sharpenBlurSize);
	ofRectangle tempROI = grayImage.getROI();
	grayImage.resetROI();
	grayImage.absDiff(temp);
	grayImage.setROI(tempROI);
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
			saveMesh();
#endif
#endif
		}
	}
}

void testApp::saveMesh() {
	int w = 640;
	int h = 480;
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinect.getDistanceAt(x, y) > 0) {
				mesh.addColor(kinect.getColorAt(x,y));
				/*ofVec3f realV;
				realV.x = kinect.getWorldCoordinateAt(x, y).x;
				realV.y = (h - 1) - kinect.getWorldCoordinateAt(x, y).y;
				realV.z = 255 - kinect.getWorldCoordinateAt(x, y).z;
				mesh.addVertex(realV);*/
				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
			}
		}
	}
	mesh.save("capture/" + ofGetTimestampString() + ".kmesh");
}
