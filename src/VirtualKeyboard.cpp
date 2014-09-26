#include "VirtualKeyboard.h"
#include "AssetsManager.h"
#include <ctype.h>

using namespace ci;
using namespace ci::app;
using namespace std;

string VirtualKeyboard::secondLineCharacters[10] = {"q","w","e","r","t","y","u","i","o","p"};
string VirtualKeyboard::thirdLineCharacters[10]  = {"a","s","d","f","g","h","j","k","l","-"};
string VirtualKeyboard::fourthLineCharacters[9]  = {"z","x","c","v","b","n","m",".","_"};

string VirtualKeyboard::secondLineCharacters2[10] = {"[","]","{","}","#","%","^","*","+","="};
string VirtualKeyboard::thirdLineCharacters2[10]  = {"$","&","\\","|","~","<",">","(",")","-"};
string VirtualKeyboard::fourthLineCharacters2[10]  = {"/",":",";",",","?","!","'","\"",".","_"};


int    VirtualKeyboard::lineLength1 = 10;

float  VirtualKeyboard::_xOffset1 = 0.f;//14.0f;
float  VirtualKeyboard::_xOffset2 = 14.0f;
float  VirtualKeyboard::_xOffset3 = 13.0f;
float  VirtualKeyboard::_xOffset4 = 12.0f;
float  VirtualKeyboard::_xOffset5 = 12.0f;

ci::Vec2f  VirtualKeyboard::lineOffset1 = Vec2f(360.0f, 30.0f);
ci::Vec2f  VirtualKeyboard::lineOffset2 = Vec2f(415.0f, 122.0f);
ci::Vec2f  VirtualKeyboard::lineOffset3 = Vec2f(455.0f, 214.0f);
ci::Vec2f  VirtualKeyboard::lineOffset4 = Vec2f(510.0f, 306.0f);
ci::Vec2f  VirtualKeyboard::lineOffset5 = Vec2f(504.0f, 398.0f);

void VirtualKeyboard::setup( ci::app::WindowRef window, Vec2f _position)
{
	Font  mFont								= Font( loadFile(getAssetPath("fonts/Helvetica Neue Light.ttf")), 32 );

	gl::Texture backspaceBtnTex				= gl::Texture( loadImage( loadAsset(  "keyboard/backBtn.png" )));
	gl::Texture _simple						= gl::Texture( loadImage( loadAsset(   "keyboard/_simpleBtn.png" )));	
	gl::Texture	yaBtnTex					= gl::Texture( loadImage( loadAsset(   "keyboard/ya.png")));
	gl::Texture ramBtnTex					= gl::Texture( loadImage( loadAsset(   "keyboard/ram.png")));
	gl::Texture mailBtnTex					= gl::Texture( loadImage( loadAsset(   "keyboard/mail.png" )));
	gl::Texture gmailBtnTex					= gl::Texture( loadImage( loadAsset(   "keyboard/gmail.png" )));
	
	gl::Texture sendBtnTex					= gl::Texture( loadImage( loadAsset(   "keyboard/send.png" )));
				shiftTex1					= gl::Texture( loadImage( loadAsset(   "keyboard/shift.png" )));
				shiftTex0					= gl::Texture( loadImage( loadAsset(   "keyboard/shift0.png")));



	gl::Texture spaceBtnTex					= gl::Texture( loadImage( loadAsset(  "keyboard2/k3.png" )));


	changeKeyboardTex1						= gl::Texture( loadImage( loadAsset(   "keyboard2/k2.png" )));
	changeKeyboardTex2						= gl::Texture( loadImage( loadAsset(   "keyboard2/k2.png")));



	Vec2f shift_Y							= Vec2f(0.0f, 0.0f);
	float _width = 86.0f;

	
	lineOffset1 = Vec2f(0.0f, 0.0f);
	lineOffset2 = Vec2f(65.0f, 92.0f);
	lineOffset3 = Vec2f(105.0f, 2*92.0f);
	lineOffset4 = Vec2f(150.0f, 3*92.0f);
	lineOffset5 = Vec2f(194.0f, 4*92.0f);
	
	for (auto i = 0; i < lineLength1; i++)
	{
		Button* btn = new Button();
		btn->setup(_simple, mFont, to_string(i));
		btn->setScreenField(lineOffset1 + Vec2f(i*(_xOffset1 + _width), 0.0f)+shift_Y);		
		//console()<<"  id::  "<< btn->getBtnId()<<std::endl;
		buttonsMainKeyboard.push_back( btn );
		buttonsSecondKeyboard.push_back( btn );
	}

	shift = new Button();
	shift->setup(shiftTex0, mFont, "shift", false);
	shift->setScreenField(lineOffset4 - Vec2f(100.0f, 0.0f) + shift_Y);
	buttonsMainKeyboard.push_back(shift);
	
	Button * backspaceBtn = new Button();
	backspaceBtn->setup(backspaceBtnTex, mFont, "back", false);
	backspaceBtn->setScreenField(lineOffset1 + Vec2f(10.0f*(_xOffset1 + _width),0.0f)+shift_Y);	
	buttonsMainKeyboard.push_back(backspaceBtn);
	buttonsSecondKeyboard.push_back( backspaceBtn );

	for (size_t i = 0, ilen = secondLineCharacters->size(); i < 10; i++)
	{
		Button * btn = new Button();
		btn->setup(_simple, mFont, secondLineCharacters[i]);
		btn->setScreenField(lineOffset2 + Vec2f(i*(_xOffset2 + _width), 0.0f) + shift_Y);		
		buttonsMainKeyboard.push_back( btn );
	}

	for (size_t i = 0, ilen = secondLineCharacters2->size(); i < 10; i++)
	{
		Button * btn = new Button();
		btn->setup(_simple, mFont, secondLineCharacters2[i]);
		btn->setScreenField(lineOffset2 + Vec2f(i*(_xOffset2 + _width), 0.0f) + shift_Y);		
		buttonsSecondKeyboard.push_back( btn );
	}

	for (size_t i = 0, ilen = thirdLineCharacters->size(); i < 10; i++)
	{
		Button * btn = new Button();
		btn->setup(_simple, mFont, thirdLineCharacters[i]);
		btn->setScreenField(lineOffset3 + Vec2f(i*(_xOffset3 + _width), 0.0f) + shift_Y);		
		buttonsMainKeyboard.push_back( btn );
	}

	for (size_t i = 0, ilen = thirdLineCharacters2->size(); i < 10; i++)
	{
		Button * btn = new Button();
		btn->setup(_simple, mFont, thirdLineCharacters2[i]);
		btn->setScreenField(lineOffset3 + Vec2f(i*(_xOffset3 + _width), 0.0f) + shift_Y);		
		buttonsSecondKeyboard.push_back( btn );
	}

	for (size_t i = 0, ilen = fourthLineCharacters->size(); i < 9; i++)
	{
		Button * btn = new Button();
		btn->setup(_simple, mFont, fourthLineCharacters[i]);
		btn->setScreenField(lineOffset4 + Vec2f(i*(_xOffset4 + _width), 0.0f)+ shift_Y);		
		buttonsMainKeyboard.push_back( btn );
	}

	for (size_t i = 0, ilen = fourthLineCharacters2->size(); i < 10; i++)
	{
		Button * btn = new Button();
		btn->setup(_simple, mFont, fourthLineCharacters2[i]);
		btn->setScreenField(lineOffset4 + Vec2f((i)*(_xOffset4 + _width), 0.0f)+ shift_Y - Vec2f(_xOffset4 + _width, 0.0f));		
		buttonsSecondKeyboard.push_back( btn );
	}

	Button *dogBtn = new Button(), *spaceBtn = new Button(), *sendBtn = new Button(), *mailBtn = new Button(), *gmailBtn= new Button(), *yaBtn= new Button(), *ramBtn= new Button();
	
	changeKeyboardBtn = new Button();

	changeKeyboardBtn->setup(changeKeyboardTex1, mFont, "#+=");
	changeKeyboardBtn->setScreenField(lineOffset5 + Vec2f(0.0f*(_xOffset5 + _width) - 170.0f, 0.0f)+ shift_Y);	
	buttonsMainKeyboard.push_back(changeKeyboardBtn);
	buttonsSecondKeyboard.push_back(changeKeyboardBtn);

	dogBtn->setup(_simple, mFont, "@");
	dogBtn->setScreenField(lineOffset5 + Vec2f(0.0f*(_xOffset5 + _width), 0.0f)+ shift_Y);	
	buttonsMainKeyboard.push_back(dogBtn);
	buttonsSecondKeyboard.push_back(dogBtn);

	spaceBtn->setup(spaceBtnTex, mFont, " ", false);
	spaceBtn->setScreenField(lineOffset5 + Vec2f((_xOffset5 + _width), 0.0f)+ shift_Y);	
	buttonsMainKeyboard.push_back(spaceBtn);
	buttonsSecondKeyboard.push_back( spaceBtn );

	sendBtn->setup(sendBtnTex, mFont, "send", false);
	Vec2f pos = lineOffset5 + Vec2f((_xOffset5  + _width), 0.0f) + shift_Y;
	sendBtn->setScreenField(pos+ Vec2f(_xOffset5 + spaceBtnTex.getWidth(), 0.0f));	
	buttonsMainKeyboard.push_back(sendBtn);
	buttonsSecondKeyboard.push_back( sendBtn );



	mailBtn->setup(mailBtnTex, mFont, "mail.ru", false);
	pos = Vec2f(1295.0f, 30)+shift_Y;
	mailBtn->setScreenField(pos);	
	buttonsMainKeyboard.push_back(mailBtn);
	buttonsSecondKeyboard.push_back( mailBtn );

	gmailBtn->setup(gmailBtnTex, mFont, "gmail.com", false);
	pos = Vec2f(1295.0f, 122.0f) + shift_Y;
	gmailBtn->setScreenField(pos);	
	buttonsMainKeyboard.push_back(gmailBtn);
	buttonsSecondKeyboard.push_back( gmailBtn );

	yaBtn->setup(yaBtnTex, mFont, "yandex.ru", false);
	pos = Vec2f(1295.0f, 214.0f) + shift_Y;
	yaBtn->setScreenField(pos);	
	buttonsMainKeyboard.push_back(yaBtn);
	buttonsSecondKeyboard.push_back( yaBtn );

	ramBtn->setup(ramBtnTex, mFont, "rambler.ru", false);
	pos = Vec2f(1295.0f, 306.0f) + shift_Y;
	ramBtn->setScreenField(pos);	
	buttonsMainKeyboard.push_back(ramBtn);
	buttonsSecondKeyboard.push_back( ramBtn );

	mainWindow = window;
	position = _position;

	show();
}

void VirtualKeyboard::show()
{
	isShiftDown = false;
	isKeyBoardChangeDown = false;
	lastCode = "NONE";
	activeKeyboard = &buttonsMainKeyboard;
}

void VirtualKeyboard::removeHandlers()
{
	MouseUpCon.disconnect( );
	MouseDownCon.disconnect();
	KeyDownCon.disconnect();
}

void VirtualKeyboard::closeKeyboard()
{
	keyboardClosedSignal();
}

void VirtualKeyboard::initKeyboard()
{	
	MouseDownCon   = mainWindow->getSignalMouseDown().connect( std::bind( &VirtualKeyboard::MouseDown, this, std::placeholders::_1 ) );
	MouseUpCon	   = mainWindow->getSignalMouseUp().connect(   std::bind( &VirtualKeyboard::MouseUp,   this, std::placeholders::_1 ) );
	KeyDownCon	   =  mainWindow->getSignalKeyDown().connect(	  std::bind( &VirtualKeyboard::KeyDown,	  this, std::placeholders::_1 ) );	

	keyboardReadySignal();
}

void VirtualKeyboard::draw( )
{
	gl::pushMatrices();
	
	for( auto item = activeKeyboard->begin(); item != activeKeyboard->end(); ++item )
	{
		(*item)->draw();
	}	

	gl::popMatrices();
}

void VirtualKeyboard::KeyDown( KeyEvent event  )
{

}

void VirtualKeyboard::MouseUp( MouseEvent &event )
{
	for( auto item = activeKeyboard->begin(); item != activeKeyboard->end(); ++item )
	{
		(*item)->up();
	}	
}

void VirtualKeyboard::setPosition(Vec2f _vec )
{
	position = _vec;
}

void VirtualKeyboard::MouseDown( MouseEvent &event )
{
	//Vec2f pos = position;
	lastCode ="-1";
	Vec2f coords   = event.getPos() - position;

	for( auto item = activeKeyboard->begin(); item != activeKeyboard->end(); ++item )
	{
		if((*item)->contains(coords))
		{
			lastCode = (*item)->getBtnId();
			(*item)->down();
			if (lastCode == "shift")
			{
				changeShiftMode();
				
			}
			else if (lastCode == "#+=" || lastCode == "ABC")
			{
				changeKeyboardMode();
			}
			else	keyboardTouchSignal();

			//console()<<" last code ::  "<<lastCode<<std::endl;
			event.setHandled(true );
			break;
		}
	}
}
string VirtualKeyboard::getLastCode()
{
	return  lastCode;
}

void VirtualKeyboard::changeShiftMode()
{
	isShiftDown = !isShiftDown;
	shift->texture = isShiftDown ? shiftTex1 : shiftTex0;

	for( auto item = activeKeyboard->begin(); item != activeKeyboard->end(); ++item )
	{			
		if((*item)->getBtnId().size()!=1) continue;
		char letter = (*item)->getBtnId()[0];
		if (isalpha(letter)) 
		{			
			string oneChar = "";
			if (isShiftDown)			
				oneChar.append(1, toupper(letter));				
			else
				oneChar.append(1, tolower(letter));	

			(*item)->setBtnId(oneChar);
		}
	}	
}

void VirtualKeyboard::changeKeyboardMode()
{
	isKeyBoardChangeDown  = !isKeyBoardChangeDown;

	if (isKeyBoardChangeDown)
	{
		changeKeyboardBtn->setBtnId("ABC");
		activeKeyboard = &buttonsSecondKeyboard;
	}
	else
	{
		changeKeyboardBtn->setBtnId("#+=");
		activeKeyboard = &buttonsMainKeyboard;
	}
}

KeyEvent VirtualKeyboard::imitate_ENTER_KEY_EVENT()
{
	KeyEvent key(getWindow(), 13,13, 13, 0,13);
	return key;
}

KeyEvent VirtualKeyboard::imitate_BACKSPACE_KEY_EVENT()
{
	KeyEvent key(getWindow(),8,8,ci::app::KeyEvent::KEY_BACKSPACE, 0,8);
	return key;
}

MouseEvent VirtualKeyboard::inititateMouseEvent(Vec2f _vec)
{
	unsigned int k = 1;
	float r = 1.0f;
	uint32_t t = 1;
	return MouseEvent(getWindow(), 1, _vec.x  , _vec.y , k,  r,  t);
}

bool VirtualKeyboard::isMailCode()
{
	return lastCode == "gmail.com" || lastCode == "mail.ru" || lastCode == "yandex.ru" || lastCode == "rambler.ru";
}

bool VirtualKeyboard::isBackCode()
{
	return lastCode == "back";
}

bool VirtualKeyboard::isSendCode()
{
	return lastCode == "send";
}