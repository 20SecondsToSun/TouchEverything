#pragma once

#include "cinder/app/AppBasic.h"

namespace touchEvrth
{
	class ITouchViewOnGrid
	{
	public:		
		virtual void draw() = 0;
		virtual void viewTouch(ci::Vec2f pos) = 0;			
		virtual int getTouchCellIndex(ci::Vec2f touchPos) = 0;
		virtual void reset() = 0;
	};
}
