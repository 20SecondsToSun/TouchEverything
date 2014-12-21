#include "Button.h"

Button::Button(const Texture& _tex, const Font& _font, const string& _char, const bool& _isText)
	:texture(_tex),
	text(_char),
	font(_font),
	isText(_isText),
	code(_char),
	overColor(Color::white())
{
	if (isText)
	{
		TextLayout simple;
		simple.setFont(font);
		simple.setColor(Color::black());
		simple.addLine(_char);	
		textTexture = Texture(simple.render(true, false));	
	}	
}

void Button::setScreenField(const Vec2f& vec)
{
	field = Rectf(vec.x, vec.y, vec.x + texture.getWidth(), vec.y + texture.getHeight());	
}

void Button::down()
{	
	timeline().apply(&overColor,  Color::hex(0x9dc9f6), Color::white() , 0.5f, EaseInBack());
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
		float halfW = (texture.getWidth() - textTexture.getWidth()) * 0.5f;
		float halfH = (texture.getHeight() - textTexture.getHeight()) * 0.5f;		
		gl::draw(textTexture, Vec2f(halfW, halfH - 5.0f));		
	}

	gl::popMatrices();
}

bool Button::contains(const Vec2f& mousePoint) const
{
	return field.contains(mousePoint);
}

float Button::getY()
{
	return field.y1;
}
float Button::getHeight()
{
	return field.y2 - field.y1;
}

string  Button::getBtnId()
{
	return code;
}

void  Button::setBtnId(const string& value)
{
	if (isText)
	{
		code = value;
		TextLayout simple;
		simple.setFont( font );
		simple.setColor(Color::black());
		simple.addLine(value);	
		textTexture = Texture(simple.render(true, false ));
	}
}