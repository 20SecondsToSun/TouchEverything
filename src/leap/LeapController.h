#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include "cinder/Timer.h"
#include "Cinder-LeapMotion.h"
#include "gesture/TapGesture.h"
#include "MathTools.h"
#include "DrawTools.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace boost::signals2;

namespace leapGestures
{
	const string NONE		   =  "NONE";
	const string TWO_FINGERS   =  "TWO_FINGERS";
	const string ONE_FINGER    =  "ONE_FINGER";
	const string SWIPE_LEFT    =  "SWIPE_LEFT";
	const string SWIPE_RIGHT   =  "SWIPE_RIGHT";
	const string BIGGER		   =  "BIGGER";
	const string SMALLER	   =  "SMALLER";	
	const string HAND_OVER	   =  "HAND_OVER";		
}

namespace leapStates
{
	const string DEFAULT	   =  "DEFAULT";
	const string TOUCH_MODE    =  "TOUCH_MODE";
	const string DEBUG		   =  "DEBUG";
}

class LeapController 
{	  
public:

	struct buttonStruct
	{
		Vec3f point1;
		Vec3f point2;
	};

	signal<void(void)> leapTouchSignal;
	
	LeapController();
	~LeapController();

	void update();
	void shutdown();	

	void sleep(float seconds);
	Vec2f getTouchPosition();
	vector<Vec2f> getSaveCoordsVec();

	Vec3f getFinger3DPosition();
	Vec2f getFingerTipPosition();
	MathTools::PlaneCoeff getPlane();
	int getBtnIndex();
	buttonStruct* getBtnVec();

private:

	Leap::Vector				leapToWorld(Leap::Vector leapPoint, Leap::InteractionBox iBox);
	LeapMotion::DeviceRef		leapDevice;
	Leap::Frame					leapFrame;
	Leap::Pointable				trackedPoint;
	Leap::Vector				planePoints[3];

	LeapTapParams leapTapParams;
	TapGesture tapGesture;

	int GESTURE_ALLOW_TIMER;
	Timer gestureTimer;
	Vec2f mFingerTipPosition;
	int trackedTipID;
	int	indexToSet;	
	
	string leapTouchMode; 
	Vec3f finger3DPosition;
	int	buttonIndex;

	void onFrame(Leap::Frame frame);
	Vec2f warpPointable(const Leap::Pointable& p);

	MathTools::PlaneCoeff plane;
	buttonStruct buttonVec[6];
	
	void calcTouchPlanes();
	void recordPlanePoint();
	void swapTouchMode();	
	Vec2f getCoords();
	void setButtonPoint1();
	void setButtonPoint2();
	void deleteLastButton();
	void setGestureAllowTimer();
	void calculateFingerTipPosition();
	void fingerTapFire();	
	void computeGesture();

	bool checkGestureAllow();
	void setTrackedPoint(Leap::FingerList fingers);
	void setFinger3DPosition();	

	void setSignals();
	
	void keyDown(KeyEvent event);

	connection updateCon;	
	connection keyDownCon;
};