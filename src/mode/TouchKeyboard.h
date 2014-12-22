#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "TouchViewOnGrid.h"
#include "VirtualKeyboard.h"

using namespace vrKeyboard;

namespace touchEvrth
{
	class TouchKeyboard : public TouchViewOnGrid
	{
	public:

		TouchKeyboard():outputText("")
		{
			touchKeyBoard = new VirtualKeyboard(Vec2f(200.0f, 280.0f));
			touchKeyBoard->initKeyboard();			
		}

		~TouchKeyboard()
		{
			delete touchKeyBoard;
		}

		void draw() override
		{
			touchKeyBoard->draw();
		}

		void reset() override
		{
			outputText = "";
		}

		void viewTouch(Vec2f touchPos) override
		{			
			MouseEvent mEvent = VirtualKeyboard::inititateMouseEvent(touchPos);
			touchKeyBoard->MouseDown(mEvent);

			string code = touchKeyBoard->getLastCode();

			if (touchKeyBoard->isBackCode(code))
				outputText = outputText.substr(0, outputText.size() - 1);
			else if (code.length() == 1)
				outputText += code;	
		}

	private:
		VirtualKeyboard *touchKeyBoard;	
		string outputText;			
	};
}