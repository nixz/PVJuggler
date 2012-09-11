/*
 * KeyBoardMouseNavigation.h
 *
 *  Created on: Dec 23, 2009
 *      Author: murali
 */

#ifndef KEYBOARDMOUSENAVIGATION_H_
#define KEYBOARDMOUSENAVIGATION_H_

#include "NavigationBase.h"
#include <gadget/Type/KeyboardMouseInterface.h>
#include <gadget/Type/KeyboardMouse/KeyEvent.h>
#include <gadget/Type/KeyboardMouse/MouseEvent.h>


namespace Navigation
{

	class KeyBoardMouseNavigation: public Navigation::NavigationBase
	{
		public:
			typedef NavigationBase    Inherited;

			KeyBoardMouseNavigation ();
			KeyBoardMouseNavigation (vrj::App* pApp);
			virtual ~KeyBoardMouseNavigation ();
			void update();
			void initialize();


		private:
			void processKeyBoardEvents	(gadget::KeyEventPtr pEvent	 );
			void processMouseEvents		(gadget::MouseEventPtr pEvent);
			void convertMouseCoordinate2WorldCoordinate(gadget::MouseEventPtr pEvent,
			                                            gmtl::Vec3f& WorldPos		    );

			gadget::KeyboardMouseInterface mKeyBoardMouse;
	};

}

#endif /* KEYBOARDMOUSENAVIGATION_H_ */
