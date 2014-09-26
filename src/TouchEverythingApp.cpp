#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "LeapController.h"
#include "VirtualKeyboard.h"
#include "Utils.h"

using namespace ci;
using namespace ci::app;
using namespace std;

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
		VirtualKeyboard		touchKeyBoard;
		string				outputText;
		bool				checkCode( vector<string>& codes );
		Font				hintFont;
		LeapController*		leap;

		vector<string>		codes;

		int					pointerHalfSize;
};

void TouchEverythingApp::setup()
{
	setWindowSize(1090, 550);	

	leap = LeapController::Instance();
	leap->setup();
	leap->leapTouchSignal.connect( 
		boost::bind(&TouchEverythingApp::leapTouchSignal, this) 
		);
	
	touchKeyBoard.setup( getWindow(), Vec2f(0.0f, 0.0f));
	touchKeyBoard.initKeyboard();
	touchKeyBoard.keyboardTouchSignal.connect( 
		boost::bind(&TouchEverythingApp::keyboardTouchSignal, this) 
		);	
	
	hintFont = Font( loadFile(getAssetPath("fonts/Helvetica Neue Light.ttf")), 76 );
	outputText = "";

	pointerHalfSize = 3.5;
}

void TouchEverythingApp::keyboardTouchSignal()
{	
	
}

void TouchEverythingApp::leapTouchSignal()
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
	gl::clear( Color( 0, 0, 0 ) ); 	

	//touchKeyBoard.draw();
	leap->draw();

	//Utils::textFieldDraw(outputText, hintFont, Vec2f(0.f, 440.f), Color(1, 0, 0));
}

void TouchEverythingApp::keyDown( KeyEvent event )
{
	int index = 0;

	console()<< "  event.getCode()  "<< event.getCode()<<endl;

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
	}
}

void TouchEverythingApp::shutdown()
{
	leap->shutdown();
}

CINDER_APP_NATIVE( TouchEverythingApp, RendererGl )