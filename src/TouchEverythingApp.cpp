#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "LeapController.h"
#include "VirtualKeyboard.h"
#include "TextTools.h"

using namespace ci;
using namespace ci::gl;
using namespace ci::app;
using namespace std;
using namespace vrKeyboard;

class TouchEverythingApp : public AppNative 
{
public:
	void setup();
	void update();
	void draw();
	void shutdown();
	void keyDown(KeyEvent event);

private:
	string outputText;	
	Font hintFont;
	vector<string> codes;
	float pointerHalfSize;
	float deltaX, deltaY;
	float partX,	partY;	

	Anim<float>	alphaFade;	

	map<int, Texture> imageDictionary;
	map<int, char>	charDictionary;

	Texture	curImage;
	Texture	heinz, koz;
	Texture	bart, cart, kitty;
	Texture	white, blue, yel;	

	enum _touchMode{TEXT, IMAGE, KEYBOARD} touchMode;

	VirtualKeyboard *touchKeyBoard;

	void loadGraphicsResources();
	void initLeapController();
	void initVirtualKeyboard();
	void initApplicationParams();	

	bool checkCode(vector<string>& codes);
	void keyboardTouchSignal();	
	void leapTouchSignal();
};

void TouchEverythingApp::setup()
{
	setWindowSize(1000, 800);	

	loadGraphicsResources();
	initLeapController();
	initVirtualKeyboard();
	initApplicationParams();

	gl::enableAlphaBlending();
}

void TouchEverythingApp::loadGraphicsResources()
{
	hintFont = Font(loadFile(getAssetPath("fonts/4131341.ttf")), 140);

	heinz = Texture(loadImage( loadAsset("beers/003hein.png")));
	koz	  = Texture(loadImage( loadAsset("beers/003koz.png")));

	bart  = Texture(loadImage( loadAsset("heroes/004bart.png")));
	cart  = Texture(loadImage( loadAsset("heroes/004cart.png")));
	kitty = Texture(loadImage( loadAsset("heroes/004kitt.png")));

	white = Texture(loadImage( loadAsset("colors/002wh.png")));
	blue  = Texture(loadImage( loadAsset("colors/002blu.png")));
	yel   = Texture(loadImage( loadAsset("colors/002yel.png")));
}

void TouchEverythingApp::initLeapController()
{
	leap().setup();
	leap().leapTouchSignal.connect( 
		boost::bind(&TouchEverythingApp::leapTouchSignal, this) 
		);
}

void TouchEverythingApp::initVirtualKeyboard()
{
	touchKeyBoard = new VirtualKeyboard(Vec2f(400.0f, 380.0f));
	touchKeyBoard->initKeyboard();
	touchKeyBoard->keyboardTouchSignal.connect( 
		boost::bind(&TouchEverythingApp::keyboardTouchSignal, this) 
		);	
}

void TouchEverythingApp::initApplicationParams()
{
	outputText = "";

	pointerHalfSize = 3.5f;

	partX = 10;
	partY = 10;

	deltaX = getWindowWidth() / partX;
	deltaY= getWindowHeight() / partY;

	charDictionary[1] = 'S';
	charDictionary[2] = 'M';
	charDictionary[3] = 'E';
	charDictionary[4] = 'G';

	charDictionary[5] = 'S';
	charDictionary[6] = 'M';
	charDictionary[7] = 'E';
	charDictionary[8] = 'G';

	imageDictionary[1] = bart;
	imageDictionary[2] = kitty;
	imageDictionary[3] = cart;
	imageDictionary[4] = bart;
	imageDictionary[5] = kitty;
	imageDictionary[6] = cart;	

	touchMode = KEYBOARD;
}

void TouchEverythingApp::keyboardTouchSignal()
{	

}

void TouchEverythingApp::leapTouchSignal()
{
	Vec2f vec = leap().saveCoordsVec[leap().saveCoordsVec.size()-1];
	int i = (int)(vec.x /deltaX) + 1;
	int j = (int)(vec.y /deltaY) + 1;

	int index = i + (j - 1) * partX;

	string __x = to_string(i);
	string __y = to_string(j);

	if (touchMode == TEXT)
	{
		if(outputText.size() >= 1)
		{
			outputText +=' ';
			outputText +=' ';
		}
		outputText += charDictionary[index];
		alphaFade = 1;
		timeline().apply( &alphaFade, 1.0f, 0.0f, 1.1f, EaseOutQuad() ).delay(1.5f);
	}
	else if (touchMode == IMAGE )
	{
		curImage = imageDictionary[index];
		alphaFade = 1;
		timeline().apply( &alphaFade, 1.0f, 0.0f, 1.1f, EaseOutQuad() ).delay(1.5f);
	}
	else if (touchMode == KEYBOARD )
	{
		codes.clear();
		for (size_t i = 0; i < leap().saveCoordsVec.size(); i++)
		{
			Vec2f vec = leap().saveCoordsVec[i];
			vector<Vec2f>  pointsToCheck;
			pointsToCheck.push_back(vec);
			pointsToCheck.push_back(Vec2f(vec.x-pointerHalfSize, vec.y-pointerHalfSize));
			pointsToCheck.push_back(Vec2f(vec.x+pointerHalfSize, vec.y+pointerHalfSize));
			pointsToCheck.push_back(Vec2f(vec.x-pointerHalfSize, vec.y+pointerHalfSize));
			pointsToCheck.push_back(Vec2f(vec.x+pointerHalfSize, vec.y-pointerHalfSize));

			for (size_t i = 0; i < pointsToCheck.size(); i++)
			{
				MouseEvent mEvent = VirtualKeyboard::inititateMouseEvent(pointsToCheck[i] );
				touchKeyBoard->MouseDown(mEvent);

				if (checkCode(codes))
					codes.push_back(touchKeyBoard->getLastCode());
			}	
		}
		
		for (size_t i = 0; i < codes.size(); i++)
		{
			if (touchKeyBoard->isBackCode(codes[i]))
				outputText = outputText.substr(0, outputText.size()-1);
			else if (codes[i].length() == 1)
				outputText += codes[i];	
		}	
	}
}

bool TouchEverythingApp::checkCode( vector<string>& codes )
{
	if(touchKeyBoard->isNoneCode())
		return false;

	string code = touchKeyBoard->getLastCode();

	for (size_t i = 0; i < codes.size(); i++)		
		if (code == codes[i])
			return false;

	return true;
}


void TouchEverythingApp::update()
{

}

void TouchEverythingApp::draw()
{
	gl::clear(Color::hex(0x222222));

	if (touchMode == TEXT)
	{
		textTools().textFieldDraw(outputText, &hintFont, Vec2f(650.f, 500.f), Color::hex(0xffffff));
	}
	else if (touchMode == IMAGE )
	{
		gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaFade));
		if(curImage)
			gl::draw(curImage);
	}
	else if (touchMode == KEYBOARD )
	{
		touchKeyBoard->draw();
	}

	leap().draw();
}

void TouchEverythingApp::keyDown(KeyEvent event)
{
	switch ( event.getCode() )
	{
	case KeyEvent::KEY_q:
		quit();
		break;

	case KeyEvent::KEY_o:
		outputText = "";
		break;
	}
}

void TouchEverythingApp::shutdown()
{
	delete touchKeyBoard;
}

CINDER_APP_NATIVE( TouchEverythingApp, RendererGl )