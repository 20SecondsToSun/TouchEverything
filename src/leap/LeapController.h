#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include "cinder/Timer.h"
#include "cinder/Rand.h"
#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"
#include "cinder/params/Params.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "Cinder-LeapMotion.h"
#include "gesture/TapGesture.h"

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

struct buttonStruct
{
	Vec3f point1;
	Vec3f point2;
};

class LeapController 
{	  
public:
	signal<void(void)> leapTouchSignal;

	void setup(); 
	void update();
	void draw();
	void shutdown();	

	void resetInitParams();
	void sleep(float seconds);
	Vec2f getTouchPosition();

	static LeapController* Instance() 
	{
		return &LeapControllerState;
	};

	vector<Vec2f> saveCoordsVec;//!!

protected:
	LeapController(){};

private:
	static LeapController LeapControllerState;

	Leap::InteractionBox		iBox;
	Leap::Vector				leapToWorld(Leap::Vector leapPoint, Leap::InteractionBox iBox);
	LeapMotion::DeviceRef		leapDevice;
	Leap::Frame					leapFrame;
	Leap::Pointable				trackedPoint;
	Leap::Vector				planePoints[6];

	LeapTapParams leapTapParams;
	TapGesture tapGesture;

	MayaCamUI mMayaCam;	
	
	Vec2f correctVec;
	bool isPointerHide;	

	bool GESTURE_ALLOW;	
	int GESTURE_ALLOW_TIMER;
	Timer gestureTimer;
	Vec2f mFingerTipPosition;
	int trackedTipID;
	int	indexToSet;	
	
	string leapTouchMode; 
	Vec3f finger3DPosition;
	bool buttonX1Record, buttonX2Record;
	int	buttonIndex;

	void onFrame(Leap::Frame frame);
	Vec2f warpPointable(const Leap::Pointable& p);

	int	messageToShow;
	int	buttonsArraySize;
	Anim<float>	alphasArray[6];
	string buttonNames[6];
	PlaneCoeff planes[6];

	buttonStruct buttonVec[6];
	Font hintFont;
	vector<Texture>	texArray;

	void setDebugMode();
	void calcTouchPlanes();
	void calcTouchPlane(PlaneCoeff& planeCoeef, Vec3f point1, Vec3f point2, Vec3f point3, Vec3f point4, int sign = 1);
	void setPlanePointToRecord(int pointIndex);
	void recordPlanePoint();
	void swapTouchMode();	
	Vec2f getCoords();
	void setButtonX1();
	void setButtonX2();
	void deleteLastButton();
	void hidePointer();
	void showSymbol(int i);		
	void setGestureAllowTimer();
	void calculateFingerTipPosition();
	void fingerTapFire();
	void drawGrid();	

	void checkGestureAllow();
	void setTrackedPoint(Leap::FingerList fingers);
	void setFinger3DPosition();	

	void mouseDown(MouseEvent event);
	void mouseDrag(MouseEvent event);	
	void keyDown(KeyEvent event);
};
inline LeapController&	leap() {return *LeapController::Instance();};