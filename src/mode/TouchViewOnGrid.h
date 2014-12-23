#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Timeline.h"
#include "ITouchViewOnGrid.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

namespace touchEvrth
{
	class TouchViewOnGrid : public ITouchViewOnGrid
	{
	public:			
		TouchViewOnGrid()
		{
			pointerHalfSize = 3.5f;

			partX = 10;
			partY = 10;

			deltaX = getWindowWidth() / partX;
			deltaY = getWindowHeight() / partY;	
		}

		void reset() override
		{

		}

		int getTouchCellIndex(Vec2f touchPos) override
		{
			int i = (int)(touchPos.x / deltaX) + 1;
			int j = (int)(touchPos.y / deltaY) + 1;

			int index = i + (j - 1) * (int)partX;

			return index;
		}			

		virtual void draw() = 0;
		virtual void viewTouch(Vec2f pos) = 0;

	protected:
		float pointerHalfSize;
		float deltaX, deltaY;
		float partX, partY;		
	};
}
