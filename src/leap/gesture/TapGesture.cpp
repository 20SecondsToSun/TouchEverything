#include "TapGesture.h"

using namespace std;
using namespace ci;
using namespace ci::app;	

bool TapGesture::isFired(Vec3f finger3DPosition, Leap::Pointable trackedPoint, Vec2f mFingerTipPosition, Leap::InteractionBox iBox)
{
	float distance = mathTools().distanceToPlane(plane, finger3DPosition);

	if (distance < minDistanceToHover)
	{
		Leap::Vector velocity = trackedPoint.tipVelocity();
		Leap::Vector position = trackedPoint.tipPosition();

		if (velocity.x < minXVelocity  && velocity.y < minYVelocity  )
		{
			saveCoords = mFingerTipPosition;

			if ((velocity.magnitude()< 5.0f && distance < minDistanceToTap && gestureOut ) || (distance < 4 && gestureOut))
			{					
				initParams(mFingerTipPosition);	
				return true;
			}

			if ( leaptap > 5 )
			{	
				if ((getElapsedSeconds() - saveseconds) > maxSecondsToTap)
				{		
					saveseconds = getElapsedSeconds();					
				}

				float _zz = saveZ - leapToWorld(position, iBox).z;

				if ( _zz < 0 )
				{
					leapZ++;
					saveZ = leapToWorld(position, iBox).z;					
				}
				else
				{
					leapZ = 0;
					saveseconds = getElapsedSeconds();								
				}

				if (((leapZ > 3 && distance < minDistanceToTap+5)|| (leapZ > 2 && distance < minDistanceToTap)) && abs(velocity.z) > minZVelocity)
				{					
					initParams(mFingerTipPosition);						
					return true;
				}
			}
			else 
			{
				leaptap++;
				saveZ = leapToWorld( trackedPoint.stabilizedTipPosition(), iBox).z;
				//gestureOut = true;
			}
		}
		else
		{
			leaptap = leapZ = 0;
			saveseconds = getElapsedSeconds();		
			gestureOut = true;
		}
	}

	return false;
}

void TapGesture::initParams(Vec2f mFingerTipPosition)
{
	leaptap = leapZ = 0;
	gestureOut = false;
	saveCoords = mFingerTipPosition;		
}

Leap::Vector TapGesture::leapToWorld(Leap::Vector leapPoint, Leap::InteractionBox iBox)
{
	leapPoint.z *= -1.0; //right-hand to left-hand rule
	Leap::Vector normalized = iBox.normalizePoint(leapPoint, false);
	normalized += Leap::Vector(0.5, 0, 0.5); //recenter origin
	return normalized * 100.0; //scale
}

void TapGesture::setPlane(MathTools::PlaneCoeff _plane)
{
	plane = _plane;
};

Vec2f TapGesture::getPointPosition()
{
	return saveCoords;
}

Leap::Pointable TapGesture::geTrackedPoint(Leap::FingerList fingers)
{
	Leap::Pointable trackedPoint;

	if(isExtended)		
		trackedPoint =  fingers.extended().fingerType(Leap::Finger::Type::TYPE_INDEX)[0];		
	else		
		trackedPoint =  fingers.fingerType(Leap::Finger::Type::TYPE_INDEX)[0];	

	return trackedPoint;		
}

Vec3f TapGesture::getFinger3DPosition(Leap::Pointable trackedPoint)
{
	Vec3f finger3DPosition;

	if(isStabilized)		
		finger3DPosition = Vec3f(trackedPoint.stabilizedTipPosition().x,
			trackedPoint.stabilizedTipPosition().y, 
			trackedPoint.tipPosition().z);	
	else	
		finger3DPosition = LeapMotion::toVec3f(trackedPoint.tipPosition());
	

	return  finger3DPosition;
}