#include "LeapController.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace leapGestures;
using namespace LeapMotion;

LeapController::LeapController()
	:buttonIndex(0),
	leapTouchMode(leapStates::DEBUG)
{	
	leapDevice 	= Device::create();
	leapDevice->connectEventHandler(&LeapController::onFrame, this);

	setSignals();
	sleep(3.0f);
}

LeapController::~LeapController()
{
	updateCon.disconnect();	
	keyDownCon.disconnect();	
}

void LeapController::setSignals()
{
	updateCon    = App::get()->getSignalUpdate().connect(bind(&LeapController::update, this));
	keyDownCon   = getWindow()->getSignalKeyDown().connect(std::bind(&LeapController::keyDown, this,std::placeholders::_1));
}

void LeapController::sleep(float seconds)
{
	GESTURE_ALLOW_TIMER = seconds;
	gestureTimer.start();
}

bool LeapController::checkGestureAllow()
{
	if(!gestureTimer.isStopped())
	{
		if(gestureTimer.getSeconds() >= GESTURE_ALLOW_TIMER)
		{
			gestureTimer.stop();
			return true;
		}
		return false;
	}	

	return true;	
}

void LeapController::setTrackedPoint(Leap::FingerList fingers)
{
	if(leapTapParams.isExtended)
	{
		trackedPoint =  fingers.extended().fingerType(Leap::Finger::Type::TYPE_INDEX)[0];
	}
	else
	{
		trackedPoint =  fingers.fingerType(Leap::Finger::Type::TYPE_INDEX)[0];	
	}
}

void LeapController::setFinger3DPosition()
{
	if(leapTapParams.isStabilized)	
	{
		finger3DPosition = Vec3f(trackedPoint.stabilizedTipPosition().x,
			trackedPoint.stabilizedTipPosition().y, 
			trackedPoint.tipPosition().z);
	}
	else
	{
		finger3DPosition =  LeapMotion::toVec3f(trackedPoint.tipPosition());
	}
}

void LeapController::update()
{	
	if (!checkGestureAllow())
		return;

	auto hands = leapFrame.hands();

	for (auto handIter = hands.begin(); handIter != hands.end(); ++handIter)
	{
		auto hand = *handIter;				
		auto pointables	= hand.fingers().extended().count();

		setTrackedPoint(hand.fingers());		

		if (leapTouchMode == leapStates::TOUCH_MODE)
		{
			if (trackedPoint.isFinger())
			{
				setFinger3DPosition();
				calculateFingerTipPosition();					
				computeGesture();						
			}			
		}
		else if (leapTouchMode == leapStates::DEFAULT)
		{
			mFingerTipPosition = warpPointable(trackedPoint);
		}		
		else if (leapTouchMode == leapStates::DEBUG)
		{
			if (trackedPoint.isFinger())
			{	
				finger3DPosition = Vec3f(trackedPoint.stabilizedTipPosition().x, 
					trackedPoint.stabilizedTipPosition().y,
					trackedPoint.tipPosition().z);		
			}
		}
	}
}

void LeapController::computeGesture()
{
	tapGesture.finger3DPosition   = finger3DPosition;
	tapGesture.trackedPoint		  = trackedPoint;
	tapGesture.mFingerTipPosition = mFingerTipPosition;
	tapGesture.iBox				  = leapFrame.interactionBox();
	tapGesture.compute();

	if (tapGesture.isFired)				
		fingerTapFire();	
}

void LeapController::calculateFingerTipPosition()
{	
	float posX = abs((finger3DPosition.x - plane.point0.x)/(plane.point1.x - plane.point0.x));
	float x = posX * getWindowWidth();

	float posY = abs((finger3DPosition.y - plane.point0.y)/(plane.point2.y - plane.point0.y));
	float y = posY * getWindowHeight();

	mFingerTipPosition = Vec2f(x, y);		
}

void LeapController::fingerTapFire()
{	
	leapTouchSignal();	
	sleep(1.0);
}

void LeapController::setButtonPoint1()
{
	buttonVec[buttonIndex].point1 = LeapMotion::toVec3f(trackedPoint.stabilizedTipPosition());
}

void LeapController::setButtonPoint2()
{
	buttonVec[buttonIndex].point2  = LeapMotion::toVec3f(trackedPoint.stabilizedTipPosition());
	buttonIndex++;
}

void LeapController::deleteLastButton()
{
	if (buttonIndex)	
		buttonIndex--;	
}

void LeapController::recordPlanePoint()
{
	if (trackedPoint.isValid())
	{		
		planePoints[indexToSet] = trackedPoint.stabilizedTipPosition();	
		calcTouchPlanes();
	}
}

void LeapController::calcTouchPlanes()
{
	mathTools().calcTouchPlane(plane,
		LeapMotion::toVec3f(planePoints[0]),
		LeapMotion::toVec3f(planePoints[1]), 
		LeapMotion::toVec3f(planePoints[2]), 
		Vec3f::zero());

	tapGesture.setPlanes(plane);	
}

void LeapController::swapTouchMode()
{
	if( leapTouchMode == leapStates::DEFAULT)
	{
		leapTouchMode = leapStates::TOUCH_MODE;
	}
	else
	{
		leapTouchMode = leapStates::DEFAULT;
	}
}

Vec2f LeapController::warpPointable(const Leap::Pointable& p)
{	
	Vec3f result = Vec3f::zero();
	if ( leapDevice )
	{
		const Leap::Screen& screen = leapDevice->getController()->locatedScreens().closestScreenHit(p);
		result = LeapMotion::toVec3f( screen.intersect( p, true, 1.0f ) );
	}
	result *= Vec3f( Vec2f( getWindowSize() ), 0.0f );
	result.y = (float)getWindowHeight() - result.y;
	return result.xy();
}

void LeapController::onFrame(Leap::Frame frame)
{
	leapFrame = frame;
}

ci::Vec2f LeapController::getTouchPosition()
{
	vector<Vec2f> saveCoordsVec = tapGesture.saveCoordsVec;
	return saveCoordsVec[saveCoordsVec.size()-1];
}

vector<Vec2f> LeapController::getSaveCoordsVec()
{
	return tapGesture.saveCoordsVec;
}

Vec3f LeapController::getFinger3DPosition()
{
	return finger3DPosition;
}

Vec2f LeapController::getFingerTipPosition()
{
	return mFingerTipPosition;
}

MathTools::PlaneCoeff LeapController::getPlane()
{
	return plane;
}

int LeapController::getBtnIndex()
{
	return buttonIndex;
}

LeapController::buttonStruct* LeapController::getBtnVec()
{
	return buttonVec;
}

void LeapController::keyDown(KeyEvent event)
{
	switch (event.getCode())
	{
	case KeyEvent::KEY_1:
	case KeyEvent::KEY_2:
	case KeyEvent::KEY_3:
	case KeyEvent::KEY_4:
	case KeyEvent::KEY_5:
	case KeyEvent::KEY_6:					
		indexToSet = (event.getCode() - '0') - 1;
		break;

	case KeyEvent::KEY_x:				
		setButtonPoint1();
		break;

	case KeyEvent::KEY_y:			
		setButtonPoint2();
		break;	

	case 13:			
		recordPlanePoint();					
		break;

	case KeyEvent::KEY_BACKSPACE:		
		deleteLastButton();					
		break;

	case KeyEvent::KEY_SPACE:	
		swapTouchMode();
		break;	
	}
}