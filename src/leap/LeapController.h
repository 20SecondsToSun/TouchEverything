#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include "cinder/Timer.h"
#include "cinder/Rand.h"
#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"
#include "Cinder-LeapMotion.h"
#include "cinder/params/Params.h"
#include "TapGesture.h"

namespace leapGestures{

	const std::string    TWO_FINGERS   =  "TWO_FINGERS";
	const std::string    ONE_FINGER    =  "ONE_FINGER";
	const std::string    SWIPE_LEFT    =  "SWIPE_LEFT";
	const std::string    SWIPE_RIGHT   =  "SWIPE_RIGHT";
	const std::string    BIGGER		   =  "BIGGER";
	const std::string    SMALLER	   =  "SMALLER";
	const std::string    NONE		   =  "NONE";
	const std::string    HAND_OVER	   =  "HAND_OVER";		
}

namespace leapStates{
	const std::string    DEFAULT	   =  "DEFAULT";
	const std::string    TOUCH_MODE    =  "TOUCH_MODE";
	const std::string    DEBUG		   =  "DEBUG";
}

struct buttonStruct
{
	ci::Vec3f			 point1;
	ci::Vec3f			 point2;
};

class LeapController 
{	  
  public:
	
	void							setup(); 
	void							update();
	void							shutdown();	
	void							draw();

	void							resetInitParams();
	void							sleep(int seconds);

	static LeapController* Instance() {
			return &LeapControllerState;
		};

	void							calcTouchPlanes( );
	void							calcTouchPlane(PlaneCoeff& planeCoeef, ci::Vec3f point1, ci::Vec3f point2, ci::Vec3f point3 , ci::Vec3f point4, int sign = 1);

	void							setPlanePointToRecord(int pointIndex );
	void							recordPlanePoint( );
	void							swapTouchMode( );

	boost::signals2::signal<void(void)> leapTouchSignal;

	ci::Vec2f						getCoords();
	void							setDebugMode();

	ci::MayaCamUI					mMayaCam;

	std::vector<ci::Vec2f>			saveCoordsVec;

	ci::params::InterfaceGlRef		mParams;	
	ci::Vec2f						correctVec;

	 void							setButtonX1();
	 void							setButtonX2();
	 void							deleteLastButton();

	 void							hidePointer( );

	 bool							isPointerHide;

	 void							showSymbol(int i);


protected:
	LeapController(){};
	
 private:
	 static LeapController		LeapControllerState;

	 void 						onFrame( Leap::Frame frame );	
	 void						setGestureAllowTimer();
	 void						calculateFingerTipPosition();
	 void						fingerTapFire();
	 void						drawGrid( );

	 ci::Vec2f					warpPointable( const Leap::Pointable& p );
	 Leap::InteractionBox		iBox;
	 Leap::Vector				leapToWorld(Leap::Vector leapPoint, Leap::InteractionBox iBox);

	 bool						GESTURE_ALLOW;	
	 int						GESTURE_ALLOW_TIMER;
	
	 LeapMotion::DeviceRef		leapDevice;
	 Leap::Frame				leapFrame;
	
	 ci::Timer					gestureTimer;
	 ci::Vec2f					mFingerTipPosition;

	 int						trackedTipID;
	
	 int						indexToSet;
	 Leap::Pointable			trackedPoint;
	 std::string				leapTouchMode; 
	 ci::Vec3f					finger3DPosition;

	 LeapTapParams				leapTapParams;

	 bool						buttonX1Record, buttonX2Record;
	 int						buttonIndex;	 

	 TapGesture					tapGesture;

	 void						checkGestureAllow();
	 void						setTrackedPoint(Leap::FingerList fingers);
	 void						setFinger3DPosition();

	 int						messageToShow;

	 int						buttonsArraySize;
	 ci::Anim<float>			alphasArray[6];
	 std:: string				buttonNames[6];
	 PlaneCoeff					planes[6];
	 Leap::Vector				planePoints[6];
	 buttonStruct				buttonVec[6];

	 ci::Font					hintFont;

	 std::vector<ci::gl::Texture>	texArray;
};