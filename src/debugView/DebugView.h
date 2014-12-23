#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"
#include "LeapController.h"

using namespace ci;
using namespace ci::gl;
using namespace boost::signals2;

namespace touchEvrth
{
	class DebugView 
	{
		LeapController *leap;
		bool isVisible;
		bool isPointerHide;
		MayaCamUI mMayaCam;	

		connection mouseDownCon;
		connection mouseDragCon;
		connection keyDownCon;

		void mouseDown(MouseEvent event);
		void mouseDrag(MouseEvent event);
		void setCameraPersp();
		void keyDown(KeyEvent event);

		void hidePointer();

		void drawButtonsControlPoints();
		void drawFingerPointer();
		void drawTouchPlaneInSpace();

	public:
		DebugView(LeapController *leap);
		~DebugView();

		void draw();
		void swapVisible();
		bool getVisibility();
	};
}