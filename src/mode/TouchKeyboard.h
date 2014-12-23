#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "TouchViewOnGrid.h"
#include "VirtualKeyboard.h"
#include "TextTools.h"

using namespace vrKeyboard;

namespace touchEvrth
{
	class TouchKeyboard : public TouchViewOnGrid
	{
	public:

		TouchKeyboard():outputText("")
		{
			font = Font(loadFile(getAssetPath("fonts/4131341.ttf")), 65);

			touchKeyBoard = new VirtualKeyboard(Vec2f(100.0f, 140.0f));
			touchKeyBoard->initKeyboard();			
		}

		~TouchKeyboard()
		{
			delete touchKeyBoard;
		}

		void draw() override
		{
			touchKeyBoard->draw();
			Texture tx = textTools().getTextField(outputText, &font, Color::white());

			color(Color::white());
			gl::draw(tx, Vec2f(0.5f * (getWindowWidth() - tx.getWidth()), 15.0f));
		}

		void reset() override
		{
			outputText = "";
			setWindowSize(1250, 620);			
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
		Font font;
	};
}