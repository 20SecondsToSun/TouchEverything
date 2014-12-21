#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/Text.h"
#include "cinder/Timeline.h"
#include <boost/signals2.hpp>
#include "boost/bind.hpp"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

class Button
{
public:
	void setup(Texture _tex, Font _font, string _char, bool _isText  = true);
	void draw();
	void down();	
	void up(){};	

	bool contains(Vec2f mousePoint);			
	void setScreenField(Vec2f vec);
	Texture texture;
	float getY();
	float getHeight();

	void setBtnId(string value);
	string getBtnId();

private:
	Texture textTexture;
	string text;
	Font font;
	Rectf field;
	string code;
	bool isText;

	Anim<Color> overColor;
	Anim<ColorA> overTextColor;
};