#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/Text.h"
#include "StringTools.h"

using namespace ci;
using namespace ci::gl;
using namespace std;

class TextTools
{
public:
	
	static TextTools& getInstance()
	{ 
		static TextTools tt; 
		return tt; 
	};

	void textFieldDraw(string text, Font *font, Vec2f coords, ColorA color)
	{
		pushMatrices();
		translate(coords);
		TextLayout simple;
		simple.setFont(*font);
		simple.setColor(color);
		simple.addLine(stringTools().cp1251_to_utf8(text.c_str()));	
		Texture render = gl::Texture(simple.render(true, false));
		gl::draw(render);
		gl::popMatrices();
		gl::color(Color::white());
	}

	Texture  getTextField(string text, Font* font, ColorA color)
	{		
		ci::TextLayout simple;
		simple.clear(ColorA(1, 1, 1, 0));
		simple.setFont(*font);
		simple.setColor(color);
		simple.addLine(stringTools().cp1251_to_utf8(text.c_str()));			
		return gl::Texture(simple.render(true, false));
	}

	Texture  getTextField(const char* text, Font* font, ColorA color)
	{		
		TextLayout simple;
		simple.clear(ColorA(1, 1, 1, 0));
		simple.setFont(*font);
		simple.setColor(color);
		simple.addLine(stringTools().cp1251_to_utf8(text));			
		return Texture(simple.render(true, false));
	}
};

inline TextTools& textTools() {return TextTools::getInstance();};