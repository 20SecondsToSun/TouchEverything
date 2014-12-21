#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"

using namespace ci;

class StringTools
{
public:

	static StringTools& getInstance() { 
		static StringTools strt; 
		return strt; 
	};

	string cp1251_to_utf8(const char *str)
	{
		string res;	
		int result_u, result_c;


		result_u = MultiByteToWideChar(1251,	0,	str,-1,	0,	0);

		if (!result_u)
			return 0;

		wchar_t *ures = new wchar_t[result_u];

		if(!MultiByteToWideChar(1251,	0,	str,	-1,	ures,	result_u))
		{
			delete[] ures;
			return 0;
		}

		result_c = WideCharToMultiByte(	CP_UTF8, 0,	ures,	-1,	0,	0,	0, 0);

		if(!result_c)
		{
			delete [] ures;
			return 0;
		}

		char *cres = new char[result_c];

		if(!WideCharToMultiByte(CP_UTF8,	0,	ures,	-1,	cres,	result_c,	0, 0))
		{
			delete[] cres;
			return 0;
		}
		delete[] ures;
		res.append(cres);
		delete[] cres;
		return res;
	}
};

inline StringTools&	stringTools() { return StringTools::getInstance(); };