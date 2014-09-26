#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIO.h"
#include "cinder/Timeline.h"
#include "cinder/Json.h"
#include "Button.h"

class VirtualKeyboard
{
	public:
		
		void setup( ci::app::WindowRef window, ci::Vec2f pos);
		void show();
		void hide();
		void draw();
		void update();
		void initKeyboard();
		void removeHandlers();
		void setPosition(ci::Vec2f _vec );
		std::string getLastCode();

		boost::signals2::signal<void(void)> keyboardTouchSignal, keyboardClosedSignal, keyboardReadySignal;

		static ci::app::KeyEvent   imitate_ENTER_KEY_EVENT();
		static ci::app::KeyEvent   imitate_BACKSPACE_KEY_EVENT();
		static ci::app::MouseEvent inititateMouseEvent(ci::Vec2f _vec);

		bool	isMailCode();
		bool    isBackCode();
		bool	isSendCode();

		void MouseDown( ci::app::MouseEvent &event );
			void MouseUp( ci::app::MouseEvent &event );

	private :	
			std::vector<Button *> buttonsMainKeyboard, buttonsSecondKeyboard, *activeKeyboard;

			static std::string secondLineCharacters[10];
			static std::string thirdLineCharacters[10];
			static std::string fourthLineCharacters[9];

			static std::string secondLineCharacters2[10];
			static std::string thirdLineCharacters2[10];
			static std::string fourthLineCharacters2[10];

			static ci::Vec2f lineOffset1,lineOffset2,lineOffset3,lineOffset4, lineOffset5;
			static float _xOffset1,_xOffset2,_xOffset3,_xOffset4, _xOffset5;
			static int lineLength1, lineLength2, lineLength3, lineLength4;

			ci::app::WindowRef mainWindow;

			void KeyDown( ci::app::KeyEvent event  );
			

			void changeShiftMode();
			void changeKeyboardMode();


			std::string lastCode;
			std::string keyboardMode;

			bool isShiftDown, isKeyBoardChangeDown;


			ci::gl::Texture	 shiftTex1, shiftTex0;
			ci::gl::Texture	 changeKeyboardTex1, changeKeyboardTex2;
			
			Button*		shift, *changeKeyboardBtn;
		

			ci::Vec2f  position;
		
			void closeKeyboard();

			ci::signals::connection KeyDownCon, MouseDownCon, MouseUpCon;
};