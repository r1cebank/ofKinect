##Kinect with openFrameworks

This project have no name yet, apparently I am horrible with names. I am currently calling it ofKinect, not to be confused with `ofxKinect` addon for `openframeworks`

###Introduction

The purpose of this project is to explore features of `openframeworks` and `Kinect for Windows` sensor. The original purpose is to make a library which will detect planes using the **Kinect Sensor**.

###Current Development

Currently this program will support `Kinect for Windows` and `Kinect for Xbox` sensors from libusb library.
Due to the noise from the Kinect depth sensors, I am developing a filter to smooth out the depth stream so the later detection will be easier to accomplish.

###Project Dependencies

This project will use the following 3rd party libraries

	openframeworks
		ofxOpenCv
		ofxKinect
		ofxXmlSettings
		ofxCv
		ofxUI
	seperate library including
		ofxRGBDepth (for file save)
###Usage

Copy this project to your `myApp` folder in openframeworks directory and dependencies to `addons` directory.
Run the included project file `Visual Studio` or `Xcode`


##日本語

##Kinect （キネクト） with openframeworks （オープンフレームワークス 略称oF）
私は名前を作るとひどいですので、このプロジェクトは名前がありません。I am currently calling it ofKinect, not to be confused with `ofxKinect` addon for `openframeworks`

###紹介

###
