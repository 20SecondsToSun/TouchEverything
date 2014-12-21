#include "TapGesture.h"

using namespace std;
using namespace ci;
using namespace ci::app;
	
void TapGesture::compute()
{
	isFired = false;	
	int   planeNum = 0;
	float distance = abs(planes[planeNum].A*finger3DPosition.x + 
							planes[planeNum].B*finger3DPosition.y +
							planes[planeNum].C*finger3DPosition.z + 
							planes[planeNum].D)
							/sqrt(planes[planeNum].A*planes[planeNum].A + 
								planes[planeNum].B*planes[planeNum].B + 
								planes[planeNum].C*planes[planeNum].C);

	if (distance < leapTapParams.minDistanceToHover)
	{
		if (trackedPoint.tipVelocity().x < leapTapParams.minXVelocity  && trackedPoint.tipVelocity().y < leapTapParams.minYVelocity  )
		{
			saveCoords = mFingerTipPosition;

			if ((trackedPoint.tipVelocity().magnitude()< 5.0f && distance < leapTapParams.minDistanceToTap && gestureOut ) || (distance<4 && gestureOut))
			{	
				saveCoordsVec.push_back(mFingerTipPosition);
				isFired    = true;					
				gestureOut = false;
				initParams();	
				return;
			}

			if ( leaptap > 5 )
			{	
				if ((getElapsedSeconds() - saveseconds) > leapTapParams.maxSecondsToTap)
				{		
					saveseconds = getElapsedSeconds();
					saveCoordsVec.clear();
				}

				float _zz = saveZ - leapToWorld( trackedPoint.tipPosition(), iBox).z;

				if ( _zz < 0 )
				{
					leapZ++;
					saveZ = leapToWorld( trackedPoint.tipPosition(), iBox).z;
					saveCoordsVec.push_back(mFingerTipPosition);
				}
				else
				{
					leapZ = 0;
					saveseconds = getElapsedSeconds();
					saveCoordsVec.clear();					
				}
							
				if (((leapZ > 3 && distance < leapTapParams.minDistanceToTap+5)|| (leapZ > 2 && distance < leapTapParams.minDistanceToTap)) && abs(trackedPoint.tipVelocity().z) > leapTapParams.minZVelocity)
				{
					isFired = true;
					gestureOut = false;
					initParams();
						
					return;
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
			saveCoordsVec.clear();
			gestureOut = true;
		}
	}
}

void TapGesture::initParams()
{
	leaptap = leapZ = 0;
	saveCoords = mFingerTipPosition;
	saveCoordsVec.push_back(mFingerTipPosition);	
}

Leap::Vector TapGesture::leapToWorld(Leap::Vector leapPoint, Leap::InteractionBox iBox)
{
	leapPoint.z *= -1.0; //right-hand to left-hand rule
	Leap::Vector normalized = iBox.normalizePoint(leapPoint, false);
	normalized += Leap::Vector(0.5, 0, 0.5); //recenter origin
	return normalized * 100.0; //scale
}