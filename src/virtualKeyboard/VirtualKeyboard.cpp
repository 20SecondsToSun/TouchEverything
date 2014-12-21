#include "VirtualKeyboard.h"
#include <ctype.h>

using namespace vrKeyboard;
using namespace VirtualKeyboardKeys;

string VirtualKeyboard::secondLineCharacters[10] = {"q","w","e","r","t","y","u","i","o","p"};
string VirtualKeyboard::thirdLineCharacters[10]  = {"a","s","d","f","g","h","j","k","l","-"};
string VirtualKeyboard::fourthLineCharacters[9]  = {"z","x","c","v","b","n","m",".","_"};

string VirtualKeyboard::secondLineCharacters2[10] = {"[","]","{","}","#","%","^","*","+","="};
string VirtualKeyboard::thirdLineCharacters2[10]  = {"$","&","\\","|","~","<",">","(",")","-"};
string VirtualKeyboard::fourthLineCharacters2[10]  = {"/",":",";",",","?","!","'","\"",".","_"};

int    VirtualKeyboard::lineLength1 = 10;

float  VirtualKeyboard::_xOffset1 = 14.0f;
float  VirtualKeyboard::_xOffset2 = 14.0f;
float  VirtualKeyboard::_xOffset3 = 13.0f;
float  VirtualKeyboard::_xOffset4 = 12.0f;
float  VirtualKeyboard::_xOffset5 = 12.0f;

ci::Vec2f  VirtualKeyboard::lineOffset1 = Vec2f(360.0f, 30.0f);
ci::Vec2f  VirtualKeyboard::lineOffset2 = Vec2f(415.0f, 122.0f);
ci::Vec2f  VirtualKeyboard::lineOffset3 = Vec2f(455.0f, 214.0f);
ci::Vec2f  VirtualKeyboard::lineOffset4 = Vec2f(510.0f, 306.0f);
ci::Vec2f  VirtualKeyboard::lineOffset5 = Vec2f(504.0f, 398.0f);

VirtualKeyboard::VirtualKeyboard(Vec2f _position)
{
	Font  mFont	= Font( loadFile(getAssetPath("fonts/Helvetica Neue Light.ttf")), 32 );

	Texture backspaceBtnTex	= Texture(loadImage(loadAsset("keyboard/backBtn.png")));
	Texture _simple		    = Texture(loadImage(loadAsset("keyboard/_simpleBtn.png")));	
	Texture	yaBtnTex		= Texture(loadImage(loadAsset("keyboard/ya.png")));
	Texture ramBtnTex		= Texture(loadImage(loadAsset("keyboard/ram.png")));
	Texture mailBtnTex		= Texture(loadImage(loadAsset("keyboard/mail.png")));
	Texture gmailBtnTex		= Texture(loadImage(loadAsset("keyboard/gmail.png")));	
	Texture sendBtnTex		= Texture(loadImage(loadAsset("keyboard/send.png")));
			shiftTex1		= Texture(loadImage(loadAsset("keyboard/shift.png")));
			shiftTex0		= Texture(loadImage(loadAsset("keyboard/shift0.png")));
	Texture spaceBtnTex		= Texture(loadImage(loadAsset("keyboard2/k3.png")));
	changeKeyboardTex1		= Texture(loadImage(loadAsset("keyboard2/k2.png")));
	changeKeyboardTex2		= Texture(loadImage(loadAsset("keyboard2/k2.png")));

	Vec2f shift_Y= Vec2f::zero();
	float _width = 86.0f;
	
	lineOffset1 = Vec2f::zero();
	lineOffset2 = Vec2f(65.0f, 92.0f);
	lineOffset3 = Vec2f(105.0f, 2*92.0f);
	lineOffset4 = Vec2f(150.0f, 3*92.0f);
	lineOffset5 = Vec2f(194.0f, 4*92.0f);
	
	for (auto i = 0; i < lineLength1; i++)
	{
		Button *btn = new Button();
		btn->setup(_simple, mFont, to_string(i));
		btn->setScreenField(lineOffset1 + Vec2f(i * (_xOffset1 + _width), 0.0f) + shift_Y);		
		buttonsMainKeyboard.push_back(btn);
		buttonsSecondKeyboard.push_back(btn);
	}

	shift = new Button();
	shift->setup(shiftTex0, mFont, SHIFT_KEY, false);
	shift->setScreenField(lineOffset4 - Vec2f(100.0f, 0.0f) + shift_Y);
	buttonsMainKeyboard.push_back(shift);
	
	Button *backspaceBtn = new Button();
	backspaceBtn->setup(backspaceBtnTex, mFont, BACK_KEY, false);
	backspaceBtn->setScreenField(lineOffset1 + Vec2f(10.0f*(_xOffset1 + _width),0.0f) + shift_Y);	
	buttonsMainKeyboard.push_back(backspaceBtn);
	buttonsSecondKeyboard.push_back( backspaceBtn );

	for (size_t i = 0, ilen = secondLineCharacters->size(); i < 10; i++)
	{
		Button *btn = new Button();
		btn->setup(_simple, mFont, secondLineCharacters[i]);
		btn->setScreenField(lineOffset2 + Vec2f(i*(_xOffset2 + _width), 0.0f) + shift_Y);		
		buttonsMainKeyboard.push_back( btn );
	}

	for (size_t i = 0, ilen = secondLineCharacters2->size(); i < 10; i++)
	{
		Button *btn = new Button();
		btn->setup(_simple, mFont, secondLineCharacters2[i]);
		btn->setScreenField(lineOffset2 + Vec2f(i*(_xOffset2 + _width), 0.0f) + shift_Y);		
		buttonsSecondKeyboard.push_back( btn );
	}

	for (size_t i = 0, ilen = thirdLineCharacters->size(); i < 10; i++)
	{
		Button *btn = new Button();
		btn->setup(_simple, mFont, thirdLineCharacters[i]);
		btn->setScreenField(lineOffset3 + Vec2f(i*(_xOffset3 + _width), 0.0f) + shift_Y);		
		buttonsMainKeyboard.push_back( btn );
	}

	for (size_t i = 0, ilen = thirdLineCharacters2->size(); i < 10; i++)
	{
		Button *btn = new Button();
		btn->setup(_simple, mFont, thirdLineCharacters2[i]);
		btn->setScreenField(lineOffset3 + Vec2f(i*(_xOffset3 + _width), 0.0f) + shift_Y);		
		buttonsSecondKeyboard.push_back( btn );
	}

	for (size_t i = 0, ilen = fourthLineCharacters->size(); i < 9; i++)
	{
		Button *btn = new Button();
		btn->setup(_simple, mFont, fourthLineCharacters[i]);
		btn->setScreenField(lineOffset4 + Vec2f(i*(_xOffset4 + _width), 0.0f) + shift_Y);		
		buttonsMainKeyboard.push_back( btn );
	}

	for (size_t i = 0, ilen = fourthLineCharacters2->size(); i < 10; i++)
	{
		Button *btn = new Button();
		btn->setup(_simple, mFont, fourthLineCharacters2[i]);
		btn->setScreenField(lineOffset4 + Vec2f((i)*(_xOffset4 + _width), 0.0f)+ shift_Y - Vec2f(_xOffset4 + _width, 0.0f));		
		buttonsSecondKeyboard.push_back( btn );
	}

	Button *dogBtn = new Button(), *spaceBtn = new Button(), *sendBtn = new Button(), *mailBtn = new Button(), *gmailBtn= new Button(), *yaBtn= new Button(), *ramBtn= new Button();
	
	changeKeyboardBtn = new Button();

	changeKeyboardBtn->setup(changeKeyboardTex1, mFont, SPECIAL_SYMBOLS_KEY);
	changeKeyboardBtn->setScreenField(lineOffset5 + Vec2f(0.0f * (_xOffset5 + _width) - 170.0f, 0.0f) + shift_Y);	
	buttonsMainKeyboard.push_back(changeKeyboardBtn);
	buttonsSecondKeyboard.push_back(changeKeyboardBtn);

	dogBtn->setup(_simple, mFont, SOBAKA_KEY);
	dogBtn->setScreenField(lineOffset5 + Vec2f(0.0f*(_xOffset5 + _width), 0.0f) + shift_Y);	
	buttonsMainKeyboard.push_back(dogBtn);
	buttonsSecondKeyboard.push_back(dogBtn);

	spaceBtn->setup(spaceBtnTex, mFont, SPACE_KEY, false);
	spaceBtn->setScreenField(lineOffset5 + Vec2f((_xOffset5 + _width), 0.0f) + shift_Y);	
	buttonsMainKeyboard.push_back(spaceBtn);
	buttonsSecondKeyboard.push_back( spaceBtn );
	 
	sendBtn->setup(sendBtnTex, mFont, SEND_KEY, false);
	Vec2f pos = lineOffset5 + Vec2f((_xOffset5  + _width), 0.0f) + shift_Y;
	sendBtn->setScreenField(pos+ Vec2f(_xOffset5 + spaceBtnTex.getWidth(), 0.0f));	
	buttonsMainKeyboard.push_back(sendBtn);
	buttonsSecondKeyboard.push_back( sendBtn );
	
	position = _position;

	show();
}

void VirtualKeyboard::show()
{
	isShiftDown = false;
	isKeyBoardChangeDown = false;
	lastCode = NONE;
	activeKeyboard = &buttonsMainKeyboard;
}

void VirtualKeyboard::removeHandlers()
{
	MouseUpCon.disconnect( );
	MouseDownCon.disconnect();
	KeyDownCon.disconnect();
}

void VirtualKeyboard::closeKeyboard() const
{
	keyboardClosedSignal();
}

void VirtualKeyboard::initKeyboard()
{	
	MouseDownCon   = getWindow()->getSignalMouseDown().connect(std::bind( &VirtualKeyboard::MouseDown, this, std::placeholders::_1 ));
	MouseUpCon	   = getWindow()->getSignalMouseUp().connect(std::bind( &VirtualKeyboard::MouseUp, this, std::placeholders::_1 ));
	KeyDownCon	   = getWindow()->getSignalKeyDown().connect(std::bind( &VirtualKeyboard::KeyDown, this, std::placeholders::_1 ));	

	keyboardReadySignal();
}

void VirtualKeyboard::draw()
{
	gl::pushMatrices();	
	gl::translate(position);

	for(auto item : *activeKeyboard)
		item->draw();

	gl::popMatrices();
}

void VirtualKeyboard::KeyDown(KeyEvent event)
{

}

void VirtualKeyboard::MouseUp(MouseEvent &event)
{
	for(auto item : *activeKeyboard)	
		item->up();	
}

void VirtualKeyboard::setPosition(Vec2f vec)
{
	position = vec;
}

void VirtualKeyboard::MouseDown(MouseEvent &event)
{
	lastCode = NONE;

	Vec2f coords = event.getPos() - position;

	for(auto item : *activeKeyboard)	
	{
		if(item->contains(coords))
		{
			lastCode = item->getBtnId();
			item->down();

			if (lastCode == SHIFT_KEY)
			{
				changeShiftMode();				
			}
			else if (lastCode == SPECIAL_SYMBOLS_KEY || lastCode == LETTERS_KEY)
			{
				changeKeyboardMode();
			}
			else keyboardTouchSignal();

			event.setHandled(true);
			break;
		}
	}
}
string VirtualKeyboard::getLastCode() const
{
	return  lastCode;
}

void VirtualKeyboard::changeShiftMode()
{
	isShiftDown = !isShiftDown;
	shift->texture = isShiftDown ? shiftTex1 : shiftTex0;

	for(auto item : *activeKeyboard)
	{			
		if(item->getBtnId().size() != 1)
			continue;

		char letter = item->getBtnId()[0];

		if (isalpha(letter)) 
		{			
			string oneChar = "";
			if (isShiftDown)			
				oneChar.append(1, toupper(letter));				
			else
				oneChar.append(1, tolower(letter));	

			item->setBtnId(oneChar);
		}
	}	
}

void VirtualKeyboard::changeKeyboardMode()
{
	isKeyBoardChangeDown  = !isKeyBoardChangeDown;

	if (isKeyBoardChangeDown)
	{
		changeKeyboardBtn->setBtnId(LETTERS_KEY);
		activeKeyboard = &buttonsSecondKeyboard;
	}
	else
	{
		changeKeyboardBtn->setBtnId(SPECIAL_SYMBOLS_KEY);
		activeKeyboard = &buttonsMainKeyboard;
	}
}

KeyEvent VirtualKeyboard::imitate_ENTER_KEY_EVENT()
{
	KeyEvent key(getWindow(), 13, 13, 13, 0, 13);
	return key;
}

KeyEvent VirtualKeyboard::imitate_BACKSPACE_KEY_EVENT()
{
	KeyEvent key(getWindow(), 8, 8, ci::app::KeyEvent::KEY_BACKSPACE, 0, 8);
	return key;
}

MouseEvent VirtualKeyboard::inititateMouseEvent(Vec2f _vec)
{
	unsigned int k = 1;
	float r = 1.0f;
	uint32_t t = 1;
	return MouseEvent(getWindow(), 1, _vec.x, _vec.y, k, r, t);
}

bool VirtualKeyboard::isMailCode() const
{
	return lastCode == GMAIL_KEY  || 
		   lastCode == MAIL_KEY   ||
		   lastCode == YANDEX_KEY || 
		   lastCode == RAMBLER_KEY;
}

bool VirtualKeyboard::isBackCode() const
{
	return isBackCode(lastCode);
}

bool VirtualKeyboard::isBackCode(string code) const
{
	return code == BACK_KEY;
}

bool VirtualKeyboard::isSendCode() const
{
	return lastCode == SEND_KEY;
}

bool VirtualKeyboard::isNoneCode() const
{
	return lastCode == NONE;
}