#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "Cinder-LeapMotion.h"
#include "MathTools.h"

using namespace ci;
using namespace std;
using namespace Leap;

namespace touchEvrth
{
	namespace tapParams
	{
		static const float  MIN_DISTANCE_HOVER = 15.5f;
		static const float  MIN_DISTANCE_TAP = 5.0f;
		static const float  MAX_GESTURE_TIME = 0.65f;
		static const float  SO_CLOSE_DISTANCE = 4.0f;

		static const float  MIN_X_VELOCITY = 80.0f;
		static const float  MIN_Y_VELOCITY = 80.0f;
		static const float  MIN_Z_VELOCITY = 8.0f;

		static const float  IS_EXTENDED = false;
		static const float  IS_STABILIZED = true;

		static const int  MIN_TAP0_FRAMES = 8;
	}

	class TapGesture 
	{
	public:
		TapGesture():tap1(0), tap0(0), startTapSec(0)
		{

		}

		bool isFired(Vec3f, Pointable, Vec2f, InteractionBox);

		Pointable geTrackedPoint(FingerList fingers);
		Vec3f getFinger3DPosition(Pointable trackedPoint);

		Vec2f getPointPosition();
		void setCalibratingPlane(MathTools::PlaneCoeff plane);

		void pushPlane(MathTools::PlaneCoeff plane);
		void popPlane();
		list<MathTools::PlaneCoeff> getPlanes();

		int getTouchedIndex();

	private:
		int touchedPlaneIndex;

		MathTools::PlaneCoeff plane;
		list<MathTools::PlaneCoeff> planes;
		

		Vector fingerVelocity, fingerPosition;
		Vec2f touchApplicantPosition;

		float fDistanceToPlane;
		int	tap0, tap1;
		double lastTapZ, startTapSec;
		
		bool isGestureTimeOver();
		bool isVelocityXYLimit();
		bool isFastDetectionConditions();
		bool isFinalGesturePartSatisfy();
		void resetParams();	
		Vector leapToWorld(Vector leapPoint, InteractionBox iBox);		

		int findTouchedIndex(Vec3f finger3DPosition);
	};
}