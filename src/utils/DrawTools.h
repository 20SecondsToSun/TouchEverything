#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci;

class DrawTools
{
public:
	static DrawTools& getInstance()
	{ 
		static DrawTools strt; 
		return strt; 
	};

	void drawGrid()
	{
		float size = 500.0f, step = 50.0f;

		gl::color(Colorf(0.2f, 0.2f, 0.2f));

		for(float i=-size; i<=size; i+=step) 
		{
			gl::drawLine(Vec3f(i, 0.0f, -size), Vec3f(i, 0.0f, size));
			gl::drawLine(Vec3f(-size, 0.0f, i), Vec3f(size, 0.0f, i));
		}

		gl::color( ColorAf(0.4f, 0.4f, 0.4f, 0.3f) );

		for(float i=-size; i<=size; i+=step) 
		{
			gl::drawLine(Vec3f(i, -size, 0), Vec3f(i, size, 0.0f));
			gl::drawLine(Vec3f(-size, i, 0.0f), Vec3f(size, i, 0.0f));
		}
	}
};
inline DrawTools& drawTools() {return DrawTools::getInstance();};