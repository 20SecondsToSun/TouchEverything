#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "Cinder-LeapMotion.h"

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

struct PlaneCoeff
{
	float A;
	float B; 
	float C; 
	float D;
	ci::Vec3f point0;
	ci::Vec3f point1; 
	ci::Vec3f point2;
	ci::Vec3f point3;
};

class TapGesture 
{
  public:
		TapGesture()
		{
			leapZ =leaptap = 0;		
			saveseconds = 0;
			gestureOut = true;
		}

	  	ci::Vec3f				 finger3DPosition;
		LeapTapParams			 leapTapParams;
		void					 compute();
		bool					 isFired;
		PlaneCoeff				 planes[6];	
		Leap::Pointable			 trackedPoint;
		ci::Vec2f				 mFingerTipPosition;
		Leap::InteractionBox	 iBox;

		void setPlanes(PlaneCoeff _planes)
		{
			planes[0] = _planes;
		};

		std::vector<ci::Vec2f>	saveCoordsVec;

  private:

		int						leaptap, leapZ;
		float					saveZ, saveseconds;
		ci::Vec2f				saveCoords;
		bool					gestureOut;		
		Leap::Vector			leapToWorld(Leap::Vector leapPoint, Leap::InteractionBox iBox);
		void					initParams();
		
};