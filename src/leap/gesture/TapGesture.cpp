#include "TapGesture.h"

using namespace std;
using namespace ci;
using namespace ci::app;	
using namespace touchEvrth;
using namespace tapParams;

bool TapGesture::isFired(Vec3f finger3DPosition, Pointable trackedPoint, Vec2f mFingerTipPosition, InteractionBox iBox)
{
	fDistanceToPlane = mathTools().distanceToPlane(plane, finger3DPosition);

	if (fDistanceToPlane < MIN_DISTANCE_HOVER)
	{
		fingerVelocity = trackedPoint.tipVelocity();
		fingerPosition = trackedPoint.tipPosition();

		if (isVelocityXYLimit())
		{
			touchApplicantPosition = mFingerTipPosition;

			if (isFastDetectionConditions())
				return true;			
			else if (tap0++ > MIN_TAP0_FRAMES)
			{	
				if (isGestureTimeOver())						
					startTapSec = getElapsedSeconds();				

				double epsilonZ = lastTapZ - leapToWorld(fingerPosition, iBox).z;

				if (epsilonZ < 0)// tap gesture continue
				{
					tap1++;
					lastTapZ = leapToWorld(fingerPosition, iBox).z;		

					if (isFinalGesturePartSatisfy())											
						return true;					
				}
				else // tap gesture failed
				{
					tap1 = 0;
					startTapSec = getElapsedSeconds();								
				}				
			}
			else		
				lastTapZ = leapToWorld(trackedPoint.stabilizedTipPosition(), iBox).z;				
		}		
	}
	resetParams();
	return false;
}

bool TapGesture::isVelocityXYLimit()
{
	return fingerVelocity.x < MIN_X_VELOCITY  && fingerVelocity.y < MIN_Y_VELOCITY;
}

bool TapGesture::isFastDetectionConditions()
{
	bool cond = (fingerVelocity.magnitude() < 5.0f
		&& fDistanceToPlane < MIN_DISTANCE_TAP 
		|| fDistanceToPlane < SO_CLOSE_DISTANCE);
	if(cond)
		resetParams();

	return cond;
}

bool TapGesture::isGestureTimeOver()
{
	return getElapsedSeconds() - startTapSec > MAX_GESTURE_TIME;
}

bool TapGesture::isFinalGesturePartSatisfy()
{
	bool cond = (tap1 > 3 && fDistanceToPlane < MIN_DISTANCE_TAP + 5)
		|| (tap1 > 2 && fDistanceToPlane < MIN_DISTANCE_TAP) 
		&&  abs(fingerVelocity.z) > MIN_Z_VELOCITY;
	if(cond)
		resetParams();

	return cond;
}

void TapGesture::resetParams()
{
	tap0 = tap1 = 0;		
}

Vec2f TapGesture::getPointPosition()
{
	return touchApplicantPosition;
}

void TapGesture::setPlane(MathTools::PlaneCoeff _plane)
{
	plane = _plane;
};

Leap::Vector TapGesture::leapToWorld(Leap::Vector leapPoint, Leap::InteractionBox iBox)
{
	leapPoint.z *= -1.0; //right-hand to left-hand rule
	Leap::Vector normalized = iBox.normalizePoint(leapPoint, false);
	normalized += Leap::Vector(0.5, 0, 0.5); //recenter origin
	return normalized * 100.0; //scale
}

Leap::Pointable TapGesture::geTrackedPoint(Leap::FingerList fingers)
{
	Leap::Pointable trackedPoint;

	if(IS_EXTENDED)		
		trackedPoint =  fingers.extended().fingerType(Leap::Finger::Type::TYPE_INDEX)[0];		
	else		
		trackedPoint =  fingers.fingerType(Leap::Finger::Type::TYPE_INDEX)[0];	

	return trackedPoint;		
}

Vec3f TapGesture::getFinger3DPosition(Leap::Pointable trackedPoint)
{
	Vec3f finger3DPosition;

	if(IS_STABILIZED)		
		finger3DPosition = Vec3f(trackedPoint.stabilizedTipPosition().x,
		trackedPoint.stabilizedTipPosition().y, 
		trackedPoint.tipPosition().z);	
	else	
		finger3DPosition = LeapMotion::toVec3f(trackedPoint.tipPosition());


	return  finger3DPosition;
}