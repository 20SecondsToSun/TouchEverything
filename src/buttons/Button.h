#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/Text.h"
#include "cinder/Timeline.h"
#include "boost/bind.hpp"
#include <boost/signals2.hpp>

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

class Button
{
public:
	Button(const Texture& tex, const Font& font, const string& ch, const bool& isText = true);
	void draw();
	void down();	
	void up(){};	

	bool contains(const Vec2f& mousePoint) const;			
	void setScreenField(const Vec2f& vec);
	Texture texture;
	float getY();
	float getHeight();

	void setBtnId(const string& value);
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