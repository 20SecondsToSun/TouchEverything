#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "LeapController.h"
#include "TouchText.h"
#include "TouchImage.h"
#include "TouchKeyboard.h"
#include "DebugView.h"

using namespace ci;
using namespace ci::gl;
using namespace ci::app;
using namespace std;
using namespace touchEvrth;

class TouchEverythingApp : public AppNative 
{
public:
	void setup();
	void draw();
	void shutdown();
	void keyDown(KeyEvent event);

private:
	void initLeapController();
	void initTouchViews();
	void leapTouchSignal();
	void setMode(TouchViewOnGrid *view);

	TouchViewOnGrid *view;
	TouchText	    *textView;
	TouchImage	    *imageView;
	TouchKeyboard   *keyboardView;

	LeapController   *leap;

	DebugView *debugView;	
};

void TouchEverythingApp::setup()
{
	setWindowSize(1400, 800);	
	gl::enableAlphaBlending();
	initLeapController();
	initTouchViews();	

	debugView = new DebugView(leap);
}

void TouchEverythingApp::initLeapController()
{
	leap = new LeapController();
	leap->leapTouchSignal.connect(boost::bind(&TouchEverythingApp::leapTouchSignal, this));
}

void TouchEverythingApp::initTouchViews()
{
	textView     = new TouchText();
	imageView    = new TouchImage();
	keyboardView = new TouchKeyboard();

	setMode(keyboardView);
}

void TouchEverythingApp::setMode(TouchViewOnGrid *_view)
{
	view = _view;
	view->reset();
}

void TouchEverythingApp::leapTouchSignal()
{
	Vec2f vec = leap->getTouchPosition();
	view->viewTouch(vec);
}

void TouchEverythingApp::draw()
{
	gl::clear(Color::hex(0x222222));
	view->draw();
	debugView->draw();
}

void TouchEverythingApp::keyDown(KeyEvent event)
{
	switch (event.getCode())
	{
	case KeyEvent::KEY_q:
		quit();
		break;

	case KeyEvent::KEY_k:
		setMode(keyboardView);
		break;

	case KeyEvent::KEY_i:
		setMode(imageView);
		break;

	case KeyEvent::KEY_t:
		setMode(textView);
		break;

	case KeyEvent::KEY_r:
		view->reset();
		break;	
	}
}

void TouchEverythingApp::shutdown()
{	
	delete textView;
	delete imageView;
	delete keyboardView;
	delete leap;
}

CINDER_APP_NATIVE(TouchEverythingApp, RendererGl)