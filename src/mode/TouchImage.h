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
		TouchImage():alphaFade(0)
		{
			heinz = Texture(loadImage(loadAsset("beers/003hein.png")));
			koz	  = Texture(loadImage( loadAsset("beers/003koz.png")));

			bart  = Texture(loadImage(loadAsset("heroes/004bart.png")));
			cart  = Texture(loadImage(loadAsset("heroes/004cart.png")));
			kitty = Texture(loadImage(loadAsset("heroes/004kitt.png")));

			white = Texture(loadImage(loadAsset("colors/002wh.png")));
			blue  = Texture(loadImage(loadAsset("colors/002blu.png")));
			yel   = Texture(loadImage(loadAsset("colors/002yel.png")));

			imageDictionary[1] = bart;
			imageDictionary[2] = kitty;
			imageDictionary[3] = cart;
			imageDictionary[4] = bart;
			imageDictionary[5] = kitty;
			imageDictionary[6] = cart;
		}

		void draw() override
		{
			gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaFade));
			if(curImage)
				gl::draw(curImage);
		}

		void reset() override
		{
			alphaFade = 0;
		}

		void viewTouch(Vec2f touchPos) override
		{
			int index = getTouchCellIndex(touchPos);

			curImage = imageDictionary[index];
			alphaFade = 1;
			timeline().apply( &alphaFade, 0.0f, 1.1f, EaseOutQuad()).delay(1.5f);
		}

	private:
		Texture	curImage;
		Texture	heinz, koz;
		Texture	bart, cart, kitty;
		Texture	white, blue, yel;

		map<int, Texture> imageDictionary;
		Anim<float>	alphaFade;	
	};
}
