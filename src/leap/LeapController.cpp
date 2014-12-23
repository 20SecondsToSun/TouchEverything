#include "LeapController.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace leapGestures;
using namespace LeapMotion;

LeapController::LeapController():leapTouchMode(leapStates::DEBUG)
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
	keyDownCon   = getWindow()->getSignalKeyDown().connect(bind(&LeapController::keyDown, this, placeholders::_1));
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
	trackedPoint = tapGesture.geTrackedPoint(fingers);	
}

void LeapController::setFinger3DPosition()
{
	finger3DPosition = tapGesture.getFinger3DPosition(trackedPoint);	
}

void LeapController::update()
{	
	if (!checkGestureAllow())
		return;

	auto hands = leapFrame.hands();

	for (auto handIter = hands.begin(); handIter != hands.end(); ++handIter)
	{
		auto hand = *handIter;				
	
		setTrackedPoint(hand.fingers());	

		if (!trackedPoint.isFinger())
			continue;

		if (leapTouchMode == leapStates::TOUCH_MODE)
		{
			setFinger3DPosition();
			calculateFingerTipPosition();					
			computeGesture();						
		}		
		else if (leapTouchMode == leapStates::DEBUG)
		{
			Leap::Vector v = trackedPoint.stabilizedTipPosition();
			finger3DPosition = Vec3f(v.x, v.y, v.z);		
		}
	}
}

void LeapController::computeGesture()
{
	if(tapGesture.isFired(finger3DPosition,
						  trackedPoint,
						  mFingerTipPosition,
						  leapFrame.interactionBox()))
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
	setTouchedButtonsIndex();// if we have that ones
	leapTouchSignal();	
	sleep(0.85);
}

void LeapController::setTouchedButtonsIndex()
{
	touchedIndex = -1;

	for (int i = 0; i < buttonVec.size(); i++)
	{
		Vec3f v = LeapMotion::toVec3f(trackedPoint.stabilizedTipPosition());
		buttonStruct b = buttonVec[i];

		if(v.x > b.point1.x && v.y < b.point1.y &&
		   v.x < b.point2.x && v.y > b.point2.y)
		{
		   touchedIndex = i;
		   break;
		}
	}
}

int LeapController::getTouchedButtonsIndex()
{
	return touchedIndex;
}

void LeapController::setButtonPoint1()
{
	currentButton.point1 = LeapMotion::toVec3f(trackedPoint.stabilizedTipPosition());	
}

void LeapController::setButtonPoint2()
{
	currentButton.point2  = LeapMotion::toVec3f(trackedPoint.stabilizedTipPosition());	
}

void LeapController::pushButtonToVec()
{
	buttonVec.push_back(currentButton);
}

void LeapController::deleteLastButton()
{
	if(!buttonVec.empty())
		buttonVec.pop_back();
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
		LeapMotion::toVec3f(planePoints[2]), Vec3f::zero());

	tapGesture.setPlane(plane);	
}

void LeapController::setTouchMode(bool isDebug)
{
	if(isDebug)	
		leapTouchMode = leapStates::DEBUG;	
	else	
		leapTouchMode = leapStates::TOUCH_MODE;	
}

Vec2f LeapController::warpPointable(const Leap::Pointable& p)
{	
	Vec3f result = Vec3f::zero();
	if (leapDevice)
	{
		const Leap::Screen& screen = leapDevice->getController()->locatedScreens().closestScreenHit(p);
		result = LeapMotion::toVec3f(screen.intersect(p, true, 1.0f));
	}

	result *= Vec3f(Vec2f(getWindowSize()), 0.0f);
	result.y = (float)getWindowHeight() - result.y;
	return result.xy();
}

void LeapController::onFrame(Leap::Frame frame)
{
	leapFrame = frame;
}

ci::Vec2f LeapController::getTouchPosition()
{
	return tapGesture.getPointPosition();	
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

vector<LeapController::buttonStruct> LeapController::getBtnVec()
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

	case KeyEvent::KEY_s:			
		pushButtonToVec();
		break;

	case 13:			
		recordPlanePoint();					
		break;

	case KeyEvent::KEY_BACKSPACE:		
		deleteLastButton();					
		break;
	}
}