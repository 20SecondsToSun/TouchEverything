#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "LeapController.h"
#include "VirtualKeyboard.h"
#include "Utils.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define						debug

class TouchEverythingApp : public AppNative 
{
  public:
	void					setup();
	void					mouseDown( MouseEvent event );	
	void					mouseDrag( MouseEvent event );

	void					keyDown( KeyEvent event );

	void					update();
	void					draw();
	void					shutdown();		

	void					keyboardTouchSignal();	
	void					leapTouchSignal();

  private:
	VirtualKeyboard			touchKeyBoard;
	string					outputText;
	bool					checkCode( vector<string>& codes );
	Font					hintFont;
	LeapController*			leap;
	vector<string>			codes;
	int						pointerHalfSize;

	float					deltaX, deltaY;
	float					partX,	partY;	

	ci::Anim<float>			alphaFade;

	void	drawGrid( );

	std::map<int, gl::Texture>	imageDictionary;
	std::map<int, char>			charDictionary;

	gl::Texture		heinz, koz, curImage;
	gl::Texture		bart, cart, kitty;
	gl::Texture		white, blue, yel;

	std::string		touchMode;

};

void TouchEverythingApp::setup()
{
	setWindowSize(1000, 800);	
	//setFullScreen(true);

	leap = LeapController::Instance();
	leap->setup();
	leap->leapTouchSignal.connect( 
		boost::bind(&TouchEverythingApp::leapTouchSignal, this) 
		);
	
	touchKeyBoard.setup( getWindow(), Vec2f(400.0f, 380.0f));
	touchKeyBoard.initKeyboard();
	touchKeyBoard.keyboardTouchSignal.connect( 
		boost::bind(&TouchEverythingApp::keyboardTouchSignal, this) 
		);	
	
	hintFont = Font( loadFile(getAssetPath("fonts/4131341.ttf")), 140 );
	outputText = "";

	pointerHalfSize = 3.5;

	partX = 10;
	partY = 10;

	deltaX = getWindowWidth() / partX;
	deltaY= getWindowHeight() / partY;

	heinz =  gl::Texture( loadImage( loadAsset(   "beers/003hein.png" )));
	koz =  gl::Texture( loadImage( loadAsset(   "beers/003koz.png" )));

	bart =  gl::Texture( loadImage( loadAsset(   "heroes/004bart.png" )));
	cart =  gl::Texture( loadImage( loadAsset(   "heroes/004cart.png" )));
	kitty =  gl::Texture( loadImage( loadAsset(   "heroes/004kitt.png" )));

	white =  gl::Texture( loadImage( loadAsset(   "colors/002wh.png" )));
	blue =  gl::Texture( loadImage( loadAsset(   "colors/002blu.png" )));
	yel =  gl::Texture( loadImage( loadAsset(   "colors/002yel.png" )));


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

	gl::enableAlphaBlending();

	touchMode = "keyboard";////"textTyping"//"image"
	
}

void TouchEverythingApp::drawGrid()
{
	float size = 500.0f, step = 50.0f;
	gl::color( Colorf(1.f, 1.f, 1.f) );

	for(float i=0; i < getWindowWidth(); i+=deltaX) 			
		gl::drawLine( Vec2f(i, 0.0f), Vec2f(i,	getWindowHeight()) );

	for(float i=0; i < getWindowHeight(); i+=deltaY) 	
		gl::drawLine( Vec2f( 0.0f, i), Vec2f(	getWindowWidth(), i) );	
}

void TouchEverythingApp::keyboardTouchSignal()
{	
	
}

void TouchEverythingApp::leapTouchSignal()
{
	Vec2f vec = leap->saveCoordsVec[leap->saveCoordsVec.size()-1];
	int i = (int)(vec.x /deltaX) + 1;
	int j =  (int)(vec.y /deltaY) + 1;

	int index = i+(j-1)*partX;

	string __x = to_string(i);
	string __y = to_string(j);

	if (touchMode == "textTyping" )
	{
		if(outputText.size()>=1)
		{
			outputText +=' ';
			outputText +=' ';
		}
		outputText += charDictionary[index];
		alphaFade = 1;
		timeline().apply( &alphaFade, 1.0f, 0.0f, 1.1f, EaseOutQuad() ).delay(1.5);
	}
	else if (touchMode == "image" )
	{
		curImage = imageDictionary[index];
		alphaFade = 1;
		timeline().apply( &alphaFade, 1.0f, 0.0f, 1.1f, EaseOutQuad() ).delay(1.5);
	}
	else if (touchMode == "keyboard" )
	{
		codes.clear();
		for (int i = 0; i < leap->saveCoordsVec.size(); i++)
		{
			Vec2f vec = leap->saveCoordsVec[i];
			vector<Vec2f>  pointsToCheck;
			pointsToCheck.push_back(vec);
			pointsToCheck.push_back(Vec2f(vec.x-pointerHalfSize, vec.y-pointerHalfSize));
			pointsToCheck.push_back(Vec2f(vec.x+pointerHalfSize, vec.y+pointerHalfSize));
			pointsToCheck.push_back(Vec2f(vec.x-pointerHalfSize, vec.y+pointerHalfSize));
			pointsToCheck.push_back(Vec2f(vec.x+pointerHalfSize, vec.y-pointerHalfSize));

			for (int i = 0; i < pointsToCheck.size(); i++)
			{
				MouseEvent mEvent = VirtualKeyboard::inititateMouseEvent(pointsToCheck[i] );
				touchKeyBoard.MouseDown(mEvent);

				if (checkCode(codes))
					codes.push_back(touchKeyBoard.getLastCode());
			}	
		}

		for (int i = 0; i < codes.size(); i++)
		{
			if (codes[i] == "back")
				outputText = outputText.substr(0, outputText.size()-1);
			else if (codes[i].length() == 1)
				outputText += codes[i];	
		}	
	}
}

bool TouchEverythingApp::checkCode( vector<string>& codes )
{
	string code = touchKeyBoard.getLastCode();
	
	if(code == "-1") return false;
	
	for (int i = 0; i < codes.size(); i++)		
		if (code == codes[i])
			return false;

	return true;
}
void TouchEverythingApp::mouseDown( MouseEvent event )
{
	leap->mMayaCam.mouseDown( event.getPos() );	
}

void TouchEverythingApp::mouseDrag( MouseEvent event )
{	
	leap->mMayaCam.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
}

void TouchEverythingApp::update()
{
	leap->update();
}

void TouchEverythingApp::draw()
{
	gl::clear(Color::hex(0x222222));

	if (touchMode == "textTyping" )
	{
		Utils::textFieldDraw(outputText, hintFont, Vec2f(650.f, 500.f), Color::hex(0xffffff));
	}
	else if (touchMode == "image" )
	{
		gl::color(ColorA(1,1,1,alphaFade));
		if(curImage)
		gl::draw(curImage);
	}
	else if (touchMode == "keyboard" )
	{
		touchKeyBoard.draw();
	}

	leap->draw();

	#ifdef debug
		drawGrid();
	#endif

}

void TouchEverythingApp::keyDown( KeyEvent event )
{
	int index = 0;

	switch ( event.getCode() )
	{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
			index = event.getCode() - '0';			
			leap->setPlanePointToRecord(index);
		break;

		case 'x':			
			leap->setButtonX1();
		break;

		case 'y':			
			leap->setButtonX2();
		break;

		case 'd':			
			leap->setDebugMode();
		break;

		case 13:			
			leap->recordPlanePoint();					
		break;

		case 8:			
			leap->deleteLastButton();					
		break;
		
		case ' ':
			leap->swapTouchMode();
		break;
		case 'q':
			quit();
		break;

		case 'h':
			leap->hidePointer();
		break;

		case '0':
			outputText = "";
		break;

		case 'u':
			leap->showSymbol(0);
		break;
		case 'i':
			leap->showSymbol(1);
		break;
		case 'o':
			leap->showSymbol(2);
		break;
		case 'p':
			leap->showSymbol(3);
		break;
	}
}

void TouchEverythingApp::shutdown()
{
	leap->shutdown();
}

CINDER_APP_NATIVE( TouchEverythingApp, RendererGl )