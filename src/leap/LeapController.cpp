#include "LeapController.h"
#include "Utils.h"
#include "cinder/Timeline.h"

using namespace std;
using namespace ci;
using namespace ci::app;

using namespace leapGestures;
using namespace LeapMotion;

LeapController LeapController::LeapControllerState;

void LeapController::setup()
{	
	hintFont = Font( loadFile(getAssetPath("fonts/Helvetica Neue Light.ttf")), 76 );
	resetInitParams();

	if (leapDevice && leapDevice->isConnected()) return;

	leapDevice 	= Device::create();
	leapDevice->connectEventHandler( &LeapController::onFrame, this );

	GESTURE_ALLOW = true;

	buttonX1Record = buttonX2Record = false;
	buttonIndex	=	0;
	messageToShow = -1;
		// set up the camera
	CameraPersp cam;
	cam.setEyePoint( Vec3f(5.0f, 300.0f, 300.0f) );
	cam.setCenterOfInterestPoint( Vec3f(0.0f, 250.0f, 0.0f) );
	cam.setPerspective( 60.0f, getWindowAspectRatio(), 1.0f, 5000.0f );
	mMayaCam.setCurrentCam( cam );

	leapTouchMode = leapStates::DEBUG;

	correctVec = Vec2f(0, 0);	
	leapTapParams.isExtended = false;
	leapTapParams.isStabilized = true;
	leapTapParams.maxSecondsToTap = 0.65f;
	leapTapParams.minDistanceToHover = 25.5;
	leapTapParams.minDistanceToTap = 9;
	leapTapParams.minXVelocity = 50;
	leapTapParams.minYVelocity = 50;
	leapTapParams.minZVelocity = 8;

	/*mParams = params::InterfaceGl::create( getWindow(), "App parameters", toPixels( Vec2i( 200, 400 ) ) );

	mParams->addParam( "isExtended", &leapTapParams.isExtended);
	mParams->addParam( "isStabilized", &leapTapParams.isStabilized);
	mParams->addSeparator();
	mParams->addParam( "minDistanceToTap", &leapTapParams.minDistanceToTap);
	mParams->addParam( "minDistanceToHover", &leapTapParams.minDistanceToHover);
	mParams->addSeparator();
	mParams->addParam( "minXVelocity", &leapTapParams.minXVelocity);
	mParams->addParam( "minYVelocity", &leapTapParams.minYVelocity);
	mParams->addParam( "minZVelocity", &leapTapParams.minZVelocity);
	mParams->addSeparator();
	mParams->addParam( "maxSecondsToTap", &leapTapParams.maxSecondsToTap);
	mParams->addSeparator();
	mParams->addParam( "correctVecX", &correctVec.x);
	mParams->addParam( "correctVecY", &correctVec.y);*/

	buttonsArraySize = 6;	

	for (int i = 0; i < buttonsArraySize; i++)
	{
		alphasArray[i] = 0;
		buttonNames[i] = "ймною "+ to_string(i+1);
	}
}

void LeapController::resetInitParams()
{
	sleep(3);
}

void LeapController::sleep(int seconds)
{
	GESTURE_ALLOW = false;
	GESTURE_ALLOW_TIMER = seconds;
	gestureTimer.start();
}

void LeapController::checkGestureAllow()
{
	GESTURE_ALLOW  = !(!gestureTimer.isStopped() && gestureTimer.getSeconds()< GESTURE_ALLOW_TIMER);	
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
	checkGestureAllow( );	
	
	const Leap::HandList& hands = leapFrame.hands();

	for ( Leap::HandList::const_iterator handIter = hands.begin(); handIter != hands.end(); ++handIter )
	{
		const Leap::Hand& hand  = *handIter;				
		auto pointables			= hand.fingers().extended().count();

		setTrackedPoint(hand.fingers());		

		 if (leapTouchMode == leapStates::TOUCH_MODE)
		{
			if (trackedPoint.isFinger())
			{
				setFinger3DPosition();
				calculateFingerTipPosition();

				if (GESTURE_ALLOW == false)	
					return;	

				tapGesture.finger3DPosition   = finger3DPosition;
				tapGesture.leapTapParams	  = leapTapParams;
				tapGesture.trackedPoint		  = trackedPoint;
				tapGesture.mFingerTipPosition = mFingerTipPosition;
				tapGesture.iBox				  = leapFrame.interactionBox();
				tapGesture.compute();

				if (tapGesture.isFired)
				{
					fingerTapFire();
				}
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

void LeapController::calculateFingerTipPosition()
{
	int planeNum = 0;
	float posX = abs((finger3DPosition.x - planes[planeNum].point0.x)/(planes[planeNum].point1.x - planes[planeNum].point0.x));
	float __x = getWindowWidth() * posX;

	float posY = abs((finger3DPosition.y - planes[planeNum].point0.y)/(planes[planeNum].point2.y - planes[planeNum].point0.y));
	float __y =  getWindowHeight()*posY;	

	mFingerTipPosition = Vec2f(__x, __y) + correctVec;		
}

void LeapController::fingerTapFire()
{
	saveCoordsVec = tapGesture.saveCoordsVec;
	leapTouchSignal();	

	for (int i = 0; i < 6; i++)
	{
		alphasArray[i] = 0;
	}
	
	for (int i = 0; i < buttonIndex; i++)
	{
		if (finger3DPosition.x >= buttonVec[i].point1.x &&
			finger3DPosition.y <= buttonVec[i].point1.y &&
			finger3DPosition.x <= buttonVec[i].point2.x &&
			finger3DPosition.y >= buttonVec[i].point2.y 
			)
		{
			console()<<"BUTTON:::::::::::::::::::::  "<< i << endl;	

			timeline().apply( &alphasArray[i], 1.0f, 0.0f, 0.3f, EaseOutQuad() );
			break;
		}	
	}
}

void LeapController::draw()
{
	gl::color(Color::hex(0xff0000));

	gl::drawSolidCircle(mFingerTipPosition, 7);
	gl::color(Color::white());

	if (leapTouchMode == leapStates::DEBUG)
	{
		gl::clear( Color( 0, 0, 0 ) ); 	
		// set up the camera 
		gl::pushMatrices();
		gl::setMatrices( mMayaCam.getCamera() );

		gl::enableDepthRead();
		gl::enableDepthWrite();

		drawGrid();		

		for (int i = 0; i < 1; i++)
		{	
			gl::color( Colorf(0.2f, 0.2f, 0.2f) );
			gl::drawLine(  planes[i].point0, planes[i].point1);
			gl::drawLine(  planes[i].point0, planes[i].point2);
			gl::drawLine(  planes[i].point1, planes[i].point3);
			gl::drawLine(  planes[i].point2, planes[i].point3);

			gl::color( Color::hex(0xffff00));
			gl::drawSphere( planes[i].point0, 0.2);
			gl::drawSphere( planes[i].point1, 0.2);
			gl::drawSphere( planes[i].point2, 0.2);
			gl::drawSphere( planes[i].point3, 0.2);
		}

		for (int i = 0; i < buttonIndex; i++)
		{
			gl::drawSphere(buttonVec[i].point1, 4);
			gl::drawSphere(buttonVec[i].point2, 4);
		}

		gl::color( Color::hex(0xff0000));
		gl::drawSphere(finger3DPosition, 2);

		gl::disableDepthRead();
		gl::disableDepthWrite();

		gl::popMatrices();
	}

	for (int i = 0; i < buttonIndex; i++)
	{		
		//console()<<"  "<<buttonNames[i]<<"  "<<alphasArray[i]<<endl;
		if (alphasArray[i]!=0)
		Utils::textFieldDraw(buttonNames[i], hintFont, Vec2f(0.f, 40.f), ColorA(1, 0, 0, alphasArray[i]));
	}
	//Utils::textFieldDraw(buttonNames[0], hintFont, Vec2f(0.f, 40.f), ColorA(1, 0, 0, 1));
	//Utils::textFieldDraw(buttonNames[1], hintFont, Vec2f(0.f, 140.f), ColorA(1, 0, 0, 1));
	
	// Draw the interface
	//mParams->draw();
}

void LeapController::drawGrid(  )
{
	float size = 500.0f, step = 50.0f;

	gl::color( Colorf(0.2f, 0.2f, 0.2f) );

	for(float i=-size; i<=size; i+=step) 
	{
		gl::drawLine( Vec3f(i, 0.0f, -size), Vec3f(i,	 0.0f, size) );
		gl::drawLine( Vec3f(-size, 0.0f, i), Vec3f(size, 0.0f, i) );
	}

	gl::color( ColorAf(0.4f, 0.4f, 0.4f, 0.3f) );

	for(float i=-size; i<=size; i+=step) 
	{
		gl::drawLine( Vec3f(i, -size, 0), Vec3f(i, size, 0.0f) );
		gl::drawLine( Vec3f(-size, i, 0.0f), Vec3f(size, i, 0.0f) );
	}
}

void LeapController::calcTouchPlanes( )
{
	calcTouchPlane(planes[0], LeapMotion::toVec3f(planePoints[0]), LeapMotion::toVec3f(planePoints[1]), LeapMotion::toVec3f(planePoints[2]), Vec3f::zero());
	tapGesture.setPlanes(planes[0]);	
}

void LeapController::calcTouchPlane(PlaneCoeff& planes, Vec3f point1, Vec3f point2, Vec3f point3, Vec3f point4, int sign )
{		
	float x1 = point1.x;
	float y1 = point1.y;
	float z1 = point1.z;

	float x2 = point2.x;
	float y2 = point2.y;
	float z2 = point2.z;

	float x3 = point3.x;
	float y3 = point3.y;
	float z3 = point3.z;

	float z4 = point4.z;

	float middle_z = (z1+z2+z3+z4)/4;

	//z1 = z2 = z3 = z4 = middle_z;


	planes.point0 = Vec3f(x1, y1, z1);
	planes.point1 = Vec3f(x2, y2, z2);
	planes.point2 = Vec3f(x3, y3, z3);
	planes.point3 = planes.point2 + sign*(planes.point1 - planes.point0);
	//planes.point3.z = z4,

	planes.A = y1 * (z2 - z3)  + y2 * (z3 - z1) + y3 *(z1 - z2);
	planes.B = z1 * (x2 - x3)  + z2 * (x3 - x1) + z3 *(x1 - x2);
	planes.C = x1 * (y2 - y3)  + x2 * (y3 - y1) + x3 *(y1 - y2);
	planes.D = -(x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1));

	//console()<<" planeA ----   :::  "<< planes.A<<endl;
	//console()<<" planeB ----   :::  "<< planes.B<<endl;
	//console()<<" planeC ----   :::  "<< planes.C<<endl;
	//console()<<" planeD ----   :::  "<< planes.D<<endl;
}

void LeapController::setButtonX1()
{
	buttonX1Record = true;
}

void LeapController::setButtonX2()
{
	buttonX2Record = true;
}

void LeapController::setPlanePointToRecord(int pointIndex )
{
	if (buttonX1Record)
	{
		buttonVec[buttonIndex].point1 = LeapMotion::toVec3f(trackedPoint.stabilizedTipPosition());
		buttonX1Record = false;
	}
	else if (buttonX2Record)
	{
		buttonVec[buttonIndex].point2  = LeapMotion::toVec3f(trackedPoint.stabilizedTipPosition());
		buttonIndex++;
		buttonX2Record = false;
	}
	else
		indexToSet = pointIndex-1;
}

void LeapController::recordPlanePoint()
{
	if (trackedPoint.isValid())
	{		
		planePoints[indexToSet] = trackedPoint.stabilizedTipPosition();	
		calcTouchPlanes();
	}
}

void LeapController::deleteLastButton()
{
	if (buttonIndex)	
		buttonIndex--;	
}

void LeapController::swapTouchMode( )
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

void LeapController::setDebugMode( )
{
	leapTouchMode = leapStates::DEBUG;
}

Vec2f LeapController::warpPointable( const Leap::Pointable& p )
{	
	Vec3f result	= Vec3f::zero();
	if ( leapDevice ) {
		const Leap::Screen& screen = leapDevice->getController()->locatedScreens().closestScreenHit( p );

		result		= LeapMotion::toVec3f( screen.intersect( p, true, 1.0f ) );
	}
	result			*= Vec3f( Vec2f( getWindowSize() ), 0.0f );
	result.y		 = (float)getWindowHeight() - result.y;
	return result.xy();
}

void LeapController::onFrame( Leap::Frame frame )
{
	leapFrame = frame;
}

void LeapController::shutdown( )
{

}