#include "Utils.h"


struct tm Utils::getCurrentTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	//char       buf[80];
	tstruct = *localtime(&now);

	return tstruct;

}

std::string  Utils::duplicateSymbol(std::string str, std::string symb)
{
	return "";
}

gl::Texture Utils::loadImageFromString(std::string value)
{	
	Buffer buff = fromBase64(value);
	auto myImage = ci::loadImage(DataSourceBuffer::create(buff), ImageSource::Options(), "png");
	return gl::Texture(myImage);
}


int Utils::spc_email_isvalid(const char *address)
{
	  int        count = 0;
	  const char *c, *domain;
	  static char *rfc822_specials = "()<>@,;:\\\"[]";

	  /* first we validate the name portion (name@domain) */
	  for (c = address;  *c;  c++) {
		if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) == 
			'\"')) {
		  while (*++c) {
			if (*c == '\"') break;
			if (*c == '\\' && (*++c == ' ')) continue;
			if (*c <= ' ' || *c >= 127) return 0;
		  }
		  if (!*c++) return 0;
		  if (*c == '@') break;
		  if (*c != '.') return 0;
		  continue;
		}
		if (*c == '@') break;
		if (*c <= ' ' || *c >= 127) return 0;
		if (strchr(rfc822_specials, *c)) return 0;
	  }
	  if (c == address || *(c - 1) == '.') return 0;

	  /* next we validate the domain portion (name@domain) */
	  if (!*(domain = ++c)) return 0;
	  do {
		if (*c == '.') {
		  if (c == domain || *(c - 1) == '.') return 0;
		  count++;
		}
		if (*c <= ' ' || *c >= 127) return 0;
		if (strchr(rfc822_specials, *c)) return 0;
	  } while (*++c);
	
	  

	 if (address[strlen(address)-1] == '.') return 0;
	// console()<<"LAST CHARTACTER  "<<address[strlen(address)-1]<<std::endl;

	  return (count >= 1);
}

bool Utils::isValidEmail(string _email)
{
	const char * mail = _email.c_str();
	return ( spc_email_isvalid(mail) == 1);
	
}
bool Utils::isValidTel(string tel)
{	
	if (tel.size()==0) return false;
	if (tel.size()<10) return false;
	for (auto i = 0; i < tel.size(); i++)
	{
		if (!isdigit(tel[i])) return false;
	}

	return true;	
}

string Utils::cp1251_to_utf8(const char *str)
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

string Utils::Utf8_to_cp1251(const char *str)
{
	string res;	
	int result_u, result_c;


	result_u = MultiByteToWideChar(CP_UTF8,	0,	str, -1, 0,	0);	
	if (!result_u)	return 0;

	wchar_t *ures = new wchar_t[result_u];

	if(!MultiByteToWideChar(CP_UTF8, 0,	str, -1, ures,	result_u))
	{
		delete[] ures;
		return 0;
	}


	result_c = WideCharToMultiByte(	1251,	0,	ures, -1,	0,	0,	0, 0);

	if(!result_c)
	{
		delete [] ures;
		return 0;
	}

	char *cres = new char[result_c];

	if(!WideCharToMultiByte(1251,	0,	ures,	-1,	cres,	result_c,	0, 0))
	{
		delete[] cres;
		return 0;
	}

	delete[] ures;
	res.append(cres);
	delete[] cres;
	return res;
}


int Utils::StringToWString(std::wstring &ws, const std::string &s)
{
    std::wstring wsTmp(s.begin(), s.end());

    ws = wsTmp;

    return 0;
}

LPWSTR Utils::ConvertToLPWSTR( const std::wstring& s )
{
  LPWSTR ws = new wchar_t[s.size()+1];
  copy( s.begin(), s.end(), ws );
  ws[s.size()] = 0;
  return ws;
}



std::wstring Utils::s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

std::string Utils::twiceSymbol(std::string __string, char __symbol)
{
	string twiceString = "";
		for (size_t  i = 0; i < __string.size(); i++)
	{
		twiceString.push_back( __string[i]);
		if (__string[i] == __symbol ) twiceString.push_back( __string[i]);
	}

	return twiceString;
}

bool Utils::readFlag(std::string path)
{
	string line;
	ifstream myfile;		
	vector<string> cam;
	myfile.open( getAssetPath(path).c_str(), ifstream::in);
	
	if (myfile.is_open())
	{
			while ( myfile.good() )
			{
				getline (myfile,line);
				cam.push_back(line);						
			}
			myfile.close();	

		 return ::atoi(cam[0].c_str()) == 1;
	}

	return false;
}


std::string Utils::urlDecode(std::string SRC) 
	{
		std::string ret;
		char ch;
		size_t  i, ii;
		for ( i=0; i<SRC.length(); i++) 
		{
			if (int(SRC[i])==37)
			{
				sscanf(SRC.substr(i+1,2).c_str(), "%x", &ii);
				ch=static_cast<char>(ii);
				ret+=ch;
				i=i+2;
			} 
			else
			{
				ret+=SRC[i];
			}
		}
		return (ret);
	}

void Utils::writeCrashLog(std::string path)
{
	string line;
	ifstream myfile;		
	vector<string> cam;
	myfile.open( getAssetPath(path).c_str(), ifstream::in);

	int value = 0;
	
	if (myfile.is_open())
	{
			while ( myfile.good() )
			{
				getline (myfile,line);
				cam.push_back(line);						
			}
			myfile.close();	

		 value = ::atoi(cam[0].c_str());
	}

	value++;

	ofstream myfile1 (getAssetPath(path).c_str());
	if (myfile1.is_open())
	{
		myfile1 << value;		
		myfile.close();
	}
	 

}

void Utils::textFieldDraw(std::string text,Font font, Vec2f coords, ColorA color)
{
	gl::pushMatrices();
	gl::translate(coords);
	TextLayout simple;
	simple.clear(ColorA(1,1,1,0));
	simple.setFont( font );
	simple.setColor(color );
	simple.addLine(cp1251_to_utf8(text.c_str()));	
	gl::draw(gl::Texture( simple.render( true, false ) ));
	gl::popMatrices();	

}

float Utils::map(float value, 
                              float istart, 
                              float istop, 
                              float ostart, 
                              float ostop) {
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}