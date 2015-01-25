#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "TouchViewOnGrid.h"

namespace touchEvrth
{
	class TouchImage : public TouchViewOnGrid
	{
	public:
		TouchImage(LeapController *_leap):alphaFade(0)
		{
			leap = _leap;

			heinz = Texture(loadImage(loadAsset("beers/003hein.png")));
			koz	  = Texture(loadImage(loadAsset("beers/003koz.png")));

			bart  = Texture(loadImage(loadAsset("heroes/004bart.png")));
			cart  = Texture(loadImage(loadAsset("heroes/004cart.png")));
			kitty = Texture(loadImage(loadAsset("heroes/004kitt.png")));

			white = Texture(loadImage(loadAsset("colors/002wh.png")));
			blue  = Texture(loadImage(loadAsset("colors/002blu.png")));
			yel   = Texture(loadImage(loadAsset("colors/002yel.png")));

			imageDictionary[0] = heinz;
			imageDictionary[1] = bart;
			imageDictionary[2] = white;
			imageDictionary[3] = koz;
			imageDictionary[4] = cart;
			imageDictionary[5] = kitty;
		}

		void draw() override
		{
			gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaFade));
			if(curImage)			
				gl::draw(curImage, getWindowBounds());
		}

		void reset() override
		{
			alphaFade = 0;
		}

		void viewTouch(Vec2f touchPos = Vec2f::zero()) override
		{
			//int index  = getTouchCellIndex(touchPos);
			int index1 = leap->getTouchedButtonsIndex();

			if (index1 != -1)
			{		
				curImage = imageDictionary[index1 % 5];
				alphaFade = 1;
				timeline().apply( &alphaFade, 0.0f, 1.1f, EaseOutQuad()).delay(1.5f);
			}
		}

	private:
		LeapController *leap;

		Texture	curImage;
		Texture	heinz, koz;
		Texture	bart, cart, kitty;
		Texture	white, blue, yel;

		map<int, Texture> imageDictionary;
		Anim<float>	alphaFade;	
	};
}
