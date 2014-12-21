#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIO.h"
#include "cinder/Timeline.h"
#include "cinder/Json.h"
#include "Button.h"

using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace boost::signals2;

namespace vrKeyboard
{
	namespace VirtualKeyboardKeys
	{
		const std::string    SHIFT_KEY			 =  "shift";
		const std::string    BACK_KEY			 =  "back";
		const std::string    SPECIAL_SYMBOLS_KEY =  "#+=";
		const std::string    SOBAKA_KEY			 =  "@";
		const std::string    SEND_KEY			 =  "send";
		const std::string    SPACE_KEY			 =  " ";
		const std::string    NONE				 =  "NONE";	
		const std::string    LETTERS_KEY		 =  "ABC";	
		const std::string    GMAIL_KEY			 =  "gmail.com";
		const std::string    MAIL_KEY			 =  "mail.ru";
		const std::string    YANDEX_KEY			 =  "yandex.ru";
		const std::string    RAMBLER_KEY		 =  "rambler.ru";
	}

	class VirtualKeyboard
	{
	public:
		VirtualKeyboard(Vec2f position);	
		void show();
		void hide();
		void draw();
		void update();
		void initKeyboard();
		void removeHandlers();
		void setPosition(Vec2f vec);	

		signal<void(void)> keyboardTouchSignal;
		signal<void(void)> keyboardClosedSignal;
		signal<void(void)> keyboardReadySignal;

		static KeyEvent imitate_ENTER_KEY_EVENT() ;
		static KeyEvent imitate_BACKSPACE_KEY_EVENT();
		static MouseEvent inititateMouseEvent(Vec2f vec);

		bool isMailCode() const;
		bool isBackCode() const;
		bool isBackCode(string code) const;
		bool isSendCode() const;
		bool isNoneCode() const;

		string getLastCode() const;

		void MouseDown(MouseEvent &event);
		void MouseUp(MouseEvent &event);

	private :	
		vector<Button*> buttonsMainKeyboard, buttonsSecondKeyboard, *activeKeyboard;

		static string secondLineCharacters[10];
		static string thirdLineCharacters[10];
		static string fourthLineCharacters[9];

		static string secondLineCharacters2[10];
		static string thirdLineCharacters2[10];
		static string fourthLineCharacters2[10];

		static Vec2f lineOffset1,lineOffset2,lineOffset3,lineOffset4, lineOffset5;
		static float _xOffset1,_xOffset2,_xOffset3,_xOffset4, _xOffset5;
		static int lineLength1, lineLength2, lineLength3, lineLength4;

		void KeyDown(KeyEvent event);

		void changeShiftMode();
		void changeKeyboardMode();
		void closeKeyboard() const;

		string lastCode;
		string keyboardMode;

		bool isShiftDown, isKeyBoardChangeDown;

		Texture shiftTex1, shiftTex0;
		Texture changeKeyboardTex1, changeKeyboardTex2;

		Button *shift, *changeKeyboardBtn;
		Vec2f  position;
		connection KeyDownCon, MouseDownCon, MouseUpCon;	
	};
}