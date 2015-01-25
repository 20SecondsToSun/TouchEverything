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

namespace touchEvrth
{
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

		Vec3f getFinger3DPosition();
		Vec2f getFingerTipPosition();
		MathTools::PlaneCoeff getPlane();	
		list<MathTools::PlaneCoeff> getPlanes();	
		vector<buttonStruct> getBtnVec();
		void setTouchMode(bool isDebug);
		int getTouchedButtonsIndex();

	private:

		Leap::Vector				leapToWorld(Leap::Vector leapPoint, Leap::InteractionBox iBox);
		LeapMotion::DeviceRef		leapDevice;
		Leap::Frame					leapFrame;
		Leap::Pointable				trackedPoint;
		Leap::Vector				planePoints[3];

		TapGesture tapGesture;

		float GESTURE_SLEEP_TIME;
		Timer gestureTimer;
		Vec2f mFingerTipPosition;
		int trackedTipID;
		int	indexToSet;	

		string leapTouchMode; 
		Vec3f finger3DPosition;	

		void onFrame(Leap::Frame frame);
		Vec2f warpPointable(const Leap::Pointable& p);

		MathTools::PlaneCoeff plane;
		list<MathTools::PlaneCoeff> planes;
		vector<buttonStruct> buttonVec;

		buttonStruct currentButton;

		void setTouchedButtonsIndex();
		int touchedIndex;

		void calcTouchPlanes();
		void recordPlanePoint();	
		Vec2f getCoords();
		void setButtonPoint1();
		void setButtonPoint2();
		void pushButtonToVec();
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
}