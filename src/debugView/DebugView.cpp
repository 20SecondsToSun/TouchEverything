#include "DebugView.h"
#include "TextTools.h"

using namespace touchEvrth;

DebugView::DebugView(LeapController *_leap)
	:isVisible(true),
	isPointerHide(false)
{
	leap = _leap;
	setCameraPersp();
	mouseDownCon = getWindow()->getSignalMouseDown().connect(std::bind(&DebugView::mouseDown, this,std::placeholders::_1));
	mouseDragCon = getWindow()->getSignalMouseDrag().connect(std::bind(&DebugView::mouseDrag, this,std::placeholders::_1));
	keyDownCon   = getWindow()->getSignalKeyDown().connect(std::bind(&DebugView::keyDown, this,std::placeholders::_1));

	hintFont = Font(loadFile(getAssetPath("fonts/4131341.ttf")), 14);
}

DebugView::~DebugView()
{
	mouseDownCon.disconnect();
	mouseDragCon.disconnect();
	keyDownCon.disconnect();
}

void DebugView::draw()
{			
	if (isVisible)
	{
		clear(Color::black()); 	
		pushMatrices();
		setMatrices(mMayaCam.getCamera());
		enableDepthRead();
		enableDepthWrite();

		drawTools().drawGrid();			
		drawTouchPlaneInSpace();
		drawButtonsControlPoints();

		color(Color::hex(0xff0000));
		drawSphere(leap->getFinger3DPosition(), 2);

		disableDepthRead();
		disableDepthWrite();
		popMatrices();

		textTools().textFieldDraw("press 1, 2, 3 keys for determine button", &hintFont, Vec2f(10.f, 10.f), Color::hex(0xffffff));
		textTools().textFieldDraw("'s' key for save button", &hintFont, Vec2f(10.f, 30.f), Color::hex(0xff0000));
		textTools().textFieldDraw("'backspace' key for delete button", &hintFont, Vec2f(10.f, 50.f), Color::hex(0xff0000));
		textTools().textFieldDraw("'d' key for switch between debug and working area", &hintFont, Vec2f(10.f, 70.f), Color::hex(0xff0000));

		gl::color( Color::hex(0xffffff));
	}
	else
		drawFingerPointer();
}

void DebugView::mouseDown(MouseEvent event)
{
	mMayaCam.mouseDown(event.getPos());	
}

void DebugView::mouseDrag(MouseEvent event)
{	
	mMayaCam.mouseDrag(event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown());
}

void DebugView::setCameraPersp()
{	
	CameraPersp cam;
	cam.setEyePoint(Vec3f(5.0f, 300.0f, 300.0f));
	cam.setCenterOfInterestPoint(Vec3f(0.0f, 250.0f, 0.0f));
	cam.setPerspective(60.0f, getWindowAspectRatio(), 1.0f, 5000.0f);
	mMayaCam.setCurrentCam(cam);
}

void DebugView::keyDown(KeyEvent event)
{
	switch (event.getCode())
	{

	case KeyEvent::KEY_h:	
		hidePointer();
		break;	
	}
}

void DebugView::swapVisible()
{
	isVisible = !isVisible;
}

bool DebugView::getVisibility()
{
	return isVisible;
}

void DebugView::hidePointer()
{
	isPointerHide = !isPointerHide;
}

void DebugView::drawButtonsControlPoints()
{
	float sphereSize = 4.0f;
	vector<LeapController::buttonStruct> buttonVec = leap->getBtnVec();

	for (size_t i = 0; i < buttonVec.size(); i++)
	{
		drawSphere(buttonVec[i].point1, sphereSize);
		drawSphere(buttonVec[i].point2, sphereSize);
	}
}

void DebugView::drawFingerPointer()
{
	color(Color::hex(0xfa8825));
	if (!isPointerHide)
		drawSolidCircle(leap->getFingerTipPosition(), 7);
}

void DebugView::drawTouchPlaneInSpace()
{
	list<MathTools::PlaneCoeff> planes = leap->getPlanes();
	MathTools::PlaneCoeff plane = leap->getPlane();

	color(Color(1,0,0));
		gl::lineWidth(3);
		drawLine(plane.point0, plane.point1);
		drawLine(plane.point0, plane.point2);
		drawLine(plane.point1, plane.point3);
		drawLine(plane.point2, plane.point3);
	gl::lineWidth(1);

	for(auto plane : planes)
	{
		color(Color::white());
		gl::lineWidth(2);
		drawLine(plane.point0, plane.point1);
		drawLine(plane.point0, plane.point2);
		drawLine(plane.point1, plane.point3);
		drawLine(plane.point2, plane.point3);
		gl::lineWidth(1);

		color(Color::hex(0xffff00));
		float sphereSize = 0.4f;	
		drawSphere(plane.point0, sphereSize);
		drawSphere(plane.point1, sphereSize);
		drawSphere(plane.point2, sphereSize);
		drawSphere(plane.point3, sphereSize);
	}
}