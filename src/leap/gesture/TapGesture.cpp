#include "TapGesture.h"

using namespace std;
using namespace ci;
using namespace ci::app;	
using namespace touchEvrth;
using namespace tapParams;

bool TapGesture::isFired(Vec3f finger3DPosition, Pointable trackedPoint, Vec2f mFingerTipPosition, InteractionBox iBox)
{	
	touchedPlaneIndex = findTouchedIndex(finger3DPosition);	

	if(touchedPlaneIndex == -1)
		return false;

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

int TapGesture::findTouchedIndex(Vec3f finger3DPosition)
{
	float fDistanceToPlane = MIN_DISTANCE_HOVER + 1;

	int index = -1, i = 0;
	
	for(auto _plane : planes)
	{
		float calced = mathTools().distanceToPlane(_plane, finger3DPosition);
		if (calced < fDistanceToPlane)
		{
			fDistanceToPlane = calced;			

			if(finger3DPosition.x > _plane.point0.x && 
			   finger3DPosition.x < _plane.point3.x && 
			   finger3DPosition.y < _plane.point0.y && 
			   finger3DPosition.y > _plane.point3.y)
			{
			  index = i;
			}
		}
		i++;
	}

	return index;
}

int TapGesture::getTouchedIndex()
{
	return touchedPlaneIndex;
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

void TapGesture::setCalibratingPlane(MathTools::PlaneCoeff _plane)
{
	plane = _plane;
};

void TapGesture::pushPlane(MathTools::PlaneCoeff _plane)
{
	planes.push_back(_plane);
};

void TapGesture::popPlane()
{
	planes.pop_back();
};

list<MathTools::PlaneCoeff> TapGesture::getPlanes()
{
	return planes;
}

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