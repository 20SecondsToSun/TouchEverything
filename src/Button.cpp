#include "Button.h"

using namespace ci;
using namespace ci::app;
using namespace std;


void Button::setup(ci::gl::Texture _tex,Font _font, string _char, bool _isText)
{
	texture = _tex;
	text = _char;
	font = _font;
	isText = _isText;
	code = _char;

	overColor = Color::white();

	if (isText == false) return;

	TextLayout simple;
	simple.setFont( font );
	simple.setColor( Color::black());
	simple.addLine(_char);	
	textTexture = gl::Texture( simple.render( true, false ) );	
}

void Button::setScreenField(Vec2f vec)
{
	field = Rectf(vec.x,vec.y,vec.x+texture.getWidth(),vec.y+texture.getHeight());	
}

void Button::down()
{	
	timeline().apply( &overColor,  Color::hex(0x9dc9f6), Color::white() , 0.5f, EaseInBack());
}

void Button::up()
{
	//overColor = Color::white();
}

void Button::draw()
{
	gl::enableAlphaBlending();
	gl::color(Color::white());

	gl::pushMatrices();
		gl::translate(field.x1, field.y1);	
		gl::color(overColor);
		gl::draw(texture);	
		gl::color(Color::black());

		if (isText)
		{
			gl::pushMatrices();
			gl::translate((texture.getWidth()-textTexture.getWidth())*0.5f,(texture.getHeight()-textTexture.getHeight())*0.5f - 5.0f);
			gl::draw(textTexture);
			gl::popMatrices();
		}

	gl::popMatrices();
}

bool Button::contains(Vec2f mousePoint)
{
	return field.contains(mousePoint);
}

float Button::getY()
{
	return field.y1;
}
float Button::getHeight()
{
	return field.y2-field.y1;
}

string  Button::getBtnId()
{
	return code;
}

void  Button::setBtnId(string value)
{
	if (isText == false) return;

	code = value;

	TextLayout simple;
	simple.setFont( font );
	simple.setColor( Color::black());
	simple.addLine(value);	
	textTexture = gl::Texture( simple.render( true, false ) );
}