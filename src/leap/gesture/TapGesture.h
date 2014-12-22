#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "Cinder-LeapMotion.h"
#include "MathTools.h"

using namespace ci;
using namespace std;

struct LeapTapParams
{
	bool isExtended;
	bool isStabilized;
	float minDistanceToHover;
	float minDistanceToTap;
	float minXVelocity;
	float minYVelocity;
	float minZVelocity;
	float maxSecondsToTap;
};

class TapGesture 
{
public:
	TapGesture()
	{
		leapZ =leaptap = 0;		
		saveseconds = 0;
		gestureOut = true;

		leapTapParams.isExtended        = false;
		leapTapParams.isStabilized      = true;
		leapTapParams.maxSecondsToTap   = 0.65f;
		leapTapParams.minDistanceToHover= 25.5;
		leapTapParams.minDistanceToTap	= 20;
		leapTapParams.minXVelocity		= 80;
		leapTapParams.minYVelocity		= 80;
		leapTapParams.minZVelocity		= 8;
	}

	Vec3f					 finger3DPosition;
	Vec2f					 mFingerTipPosition;
	LeapTapParams			 leapTapParams;
	void					 compute();
	bool					 isFired;
	MathTools::PlaneCoeff	 planes[6];	
	Leap::Pointable			 trackedPoint;
	Leap::InteractionBox	 iBox;		

	void setPlanes(MathTools::PlaneCoeff planes);
	vector<Vec2f> saveCoordsVec;

private:
	int	leaptap, leapZ;
	float saveZ, saveseconds;
	Vec2f saveCoords;
	bool gestureOut;		
	Leap::Vector leapToWorld(Leap::Vector leapPoint, Leap::InteractionBox iBox);

	void initParams();		
};