#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci;

class MathTools
{
public:
	struct PlaneCoeff
	{
		float A;
		float B; 
		float C; 
		float D;
		Vec3f point0;
		Vec3f point1; 
		Vec3f point2;
		Vec3f point3;
	};

	static MathTools& getInstance()
	{ 
		static MathTools strt; 
		return strt; 
	};

	void calcTouchPlane(PlaneCoeff& planes, Vec3f point1, Vec3f point2, Vec3f point3, Vec3f point4, int sign = 1)
	{		
		float x1 = point1.x;
		float y1 = point1.y;
		float z1 = point1.z;

		float x2 = point2.x;
		float y2 = point2.y;
		float z2 = point2.z;

		float x3 = point3.x;
		float y3 = point3.y;
		float z3 = point3.z;
		float z4 = point4.z;

		float middle_z = (z1+z2+z3+z4)/4;

		//z1 = z2 = z3 = z4 = middle_z;

		planes.point0 = Vec3f(x1, y1, z1);
		planes.point1 = Vec3f(x2, y2, z2);
		planes.point2 = Vec3f(x3, y3, z3);
		planes.point3 = planes.point2 + sign*(planes.point1 - planes.point0);
		//planes.point3.z = z4,

		planes.A = y1 * (z2 - z3)  + y2 * (z3 - z1) + y3 *(z1 - z2);
		planes.B = z1 * (x2 - x3)  + z2 * (x3 - x1) + z3 *(x1 - x2);
		planes.C = x1 * (y2 - y3)  + x2 * (y3 - y1) + x3 *(y1 - y2);
		planes.D = -(x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1));
	}
	
	float distanceToPlane(PlaneCoeff plane, Vec3f v)
	{
		return abs(plane.A * v.x + plane.B * v.y + plane.C * v.z + plane.D)/
			sqrt(plane.A * plane.A + plane.B * plane.B + plane.C * plane.C);
	}
};

inline MathTools& mathTools() {return MathTools::getInstance();};