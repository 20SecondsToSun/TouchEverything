#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "TouchViewOnGrid.h"
#include "VirtualKeyboard.h"

using namespace vrKeyboard;
//using namespace VirtualKeyboardKeys;

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
			codes.clear();
			for (size_t i = 0; i < leap().saveCoordsVec.size(); i++)
			{
				Vec2f vec = leap().saveCoordsVec[i];
				vector<Vec2f>  pointsToCheck;
				pointsToCheck.push_back(vec);
				pointsToCheck.push_back(Vec2f(-pointerHalfSize, -pointerHalfSize) + vec);
				pointsToCheck.push_back(Vec2f( pointerHalfSize,  pointerHalfSize) + vec);
				pointsToCheck.push_back(Vec2f(-pointerHalfSize,  pointerHalfSize) + vec);
				pointsToCheck.push_back(Vec2f( pointerHalfSize, -pointerHalfSize) + vec);

				for (size_t i = 0; i < pointsToCheck.size(); i++)
				{
					MouseEvent mEvent = VirtualKeyboard::inititateMouseEvent(pointsToCheck[i]);
					touchKeyBoard->MouseDown(mEvent);

					if (checkCode(codes))
						codes.push_back(touchKeyBoard->getLastCode());
				}	
			}

			for (size_t i = 0; i < codes.size(); i++)
			{
				if (touchKeyBoard->isBackCode(codes[i]))
					outputText = outputText.substr(0, outputText.size()-1);
				else if (codes[i].length() == 1)
					outputText += codes[i];	
			}	
		}
	private:
		VirtualKeyboard *touchKeyBoard;
		vector<string> codes;
		string outputText;	

		bool checkCode(vector<string>& codes)
		{
			if(touchKeyBoard->isNoneCode())
				return false;

			string code = touchKeyBoard->getLastCode();

			for (size_t i = 0; i < codes.size(); i++)		
				if (code == codes[i])
					return false;

			return true;
		}
	};
}
