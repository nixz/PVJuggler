#ifndef GAMEPADNAVIGATION_H_
#define GAMEPADNAVIGATION_H_

#include <Navigation/NavigationBase.h>
#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <vrj/Kernel/App.h>
namespace Navigation
{
	class GamepadNavigation : public NavigationBase
	{
		public:
			typedef NavigationBase    Inherited;

			GamepadNavigation (vrj::App *pApp);
			virtual ~GamepadNavigation ();
			void initialize();
			void update();
			float CorrectAnalogData(gadget::AnalogInterface AnalogIn);

		private:
			/*
			 * Left Stick Y Axis
			 * Move along Z nothing but Moving Forward and Backward
			 */
			gadget::AnalogInterface mAnologMoveAlongZ;
			/*
			 * Left Stick X Axis
			 * Rotate around Y axis nothing but Turning Left and Right
			 */
			gadget::AnalogInterface mAnologRotateWithY;

			/*
			 * LeftStick Y Axis
			 * Look Up and Down
			 */
			gadget::AnalogInterface mAnologLookUpDown;

			/*
			 *	Move Positive Y axis
			 */
			gadget::DigitalInterface mDigitalMoveUp;

			/*
			 * Move Negative Y axis
			 */
			gadget::DigitalInterface mDigitalMoveDown;

			/*
			 * To Stop the kernel
			 */
			gadget::DigitalInterface mDigitalExit;

			/*
			 * To Display Information
			 */
			gadget::DigitalInterface mDigitalInfo;


			/*
			 *  To verify the data is started flowing
			 */

			bool mHasData;
	};
}
#endif /* GAMEPADNAVIGATION_H_ */
