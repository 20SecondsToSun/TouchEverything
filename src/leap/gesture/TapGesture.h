#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "Cinder-LeapMotion.h"
#include "MathTools.h"

using namespace ci;
using namespace std;

class TapGesture 
{
public:
	TapGesture()
	{
		leapZ = 0;
		leaptap = 0;		
		saveseconds = 0;
		gestureOut = true;
		isExtended = false;
		isStabilized = true;
		maxSecondsToTap = 0.65f;
		minDistanceToHover = 25.5;
		minDistanceToTap = 20;
		minXVelocity = 80;
		minYVelocity = 80;
		minZVelocity = 8;
	}

	bool isFired(Vec3f, Leap::Pointable, Vec2f, Leap::InteractionBox);

	Leap::Pointable geTrackedPoint(Leap::FingerList fingers);
	Vec3f getFinger3DPosition(Leap::Pointable trackedPoint);

	Vec2f getPointPosition();
	void setPlane(MathTools::PlaneCoeff planes);

private:

	MathTools::PlaneCoeff plane;		
	
	int	leaptap, leapZ;
	float saveZ, saveseconds;
	Vec2f saveCoords;
	bool gestureOut;
	void initParams(Vec2f value);	

	Leap::Vector leapToWorld(Leap::Vector leapPoint, Leap::InteractionBox iBox);

	bool isExtended;
	bool isStabilized;
	float minDistanceToHover;
	float minDistanceToTap;
	float minXVelocity;
	float minYVelocity;
	float minZVelocity;
	float maxSecondsToTap;
};