#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "StringTools.h"

using namespace ci;

class TextTools
{
public:
	
	static TextTools& getInstance() { 
		static TextTools tt; 
		return tt; 
	};

	void textFieldDraw(std::string text,ci::Font *font, Vec2f coords, ColorA color)
	{
		gl::pushMatrices();
		gl::translate(coords);
		ci::TextLayout simple;
		//simple.clear(ColorA(1,1,1,0));
		simple.setFont( *font );
		simple.setColor(color );
		simple.addLine(stringTools().cp1251_to_utf8(text.c_str()));	
		gl::Texture render = gl::Texture( simple.render( true, false ) );
		gl::draw(render);
		gl::popMatrices();
		gl::color(Color::white());
	}

	gl::Texture  getTextField(std::string text,ci::Font* font, ColorA color)
	{		
		ci::TextLayout simple;
		simple.clear(ColorA(1,1,1,0));
		simple.setFont( *font );
		simple.setColor(color );
		simple.addLine(stringTools().cp1251_to_utf8(text.c_str()));			
		return gl::Texture( simple.render( true, false ) );
	}

	gl::Texture  getTextField(const char* text,ci::Font* font, ColorA color)
	{		
		ci::TextLayout simple;
		simple.clear(ColorA(1,1,1,0));
		simple.setFont( *font );
		simple.setColor(color );
		simple.addLine(stringTools().cp1251_to_utf8(text));			
		return gl::Texture( simple.render( true, false ) );
	}
};

// helper function(s) for easier access 
inline TextTools&	textTools() { return TextTools::getInstance(); };