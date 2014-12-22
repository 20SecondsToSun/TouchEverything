#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "TouchViewOnGrid.h"
#include "TextTools.h"

namespace touchEvrth
{
	class TouchText : public TouchViewOnGrid
	{
	public:

		TouchText():outputText("")
		{
			hintFont = Font(loadFile(getAssetPath("fonts/4131341.ttf")), 140);

			charDictionary[1] = 'S';
			charDictionary[2] = 'M';
			charDictionary[3] = 'E';
			charDictionary[4] = 'G';

			charDictionary[5] = 'S';
			charDictionary[6] = 'M';
			charDictionary[7] = 'E';
			charDictionary[8] = 'G';
		}

		void draw() override
		{
			textTools().textFieldDraw(outputText, &hintFont, Vec2f(650.f, 500.f), Color::hex(0xffffff));
		}

		void reset() override
		{
			outputText = "";
		}

		void viewTouch(Vec2f touchPos) override
		{
			int index = getTouchCellIndex(touchPos);

			if(outputText.size() >= 1)
			{
				outputText += ' ';
				outputText += ' ';
			}

			outputText += charDictionary[index];
			alphaFade = 1;
			timeline().apply( &alphaFade, 0.0f, 1.1f, EaseOutQuad()).delay(1.5f);
		}

	private:
		string outputText;	
		Font hintFont;
		Anim<float>	alphaFade;	
		map<int, char>	charDictionary;
	};
}
