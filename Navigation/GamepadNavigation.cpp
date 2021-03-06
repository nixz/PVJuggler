#include "GamepadNavigation.h"
//#include <mainApp.h>
namespace Navigation
{
	GamepadNavigation::GamepadNavigation (vrj::App *pApp)
						:Inherited(pApp ),
						 mHasData (false)
	{

	}

	GamepadNavigation::~GamepadNavigation ()
	{

	}

	void
	GamepadNavigation::initialize(){
		Inherited::initialize();

		mAnologMoveAlongZ.init		("GamepadStickLeftAxisY");
		mAnologRotateWithY.init		("GamepadStickLeftAxisX");
		mAnologLookUpDown.init		("GamepadStickRightAxisY");

		mDigitalMoveDown.init		("GamepadButton05Proxy");
		mDigitalMoveUp.init			("GamepadButton07Proxy");
		mDigitalInfo.init			("GamepadButton06Proxy");
		mDigitalExit.init			("GamepadButton10Proxy");
	}

	void
	GamepadNavigation::update(){

		gmtl::Vec3f x_dir ( 1.0f, 0.0f, 0.0f );
		gmtl::Vec3f y_dir ( 0.0f, 1.0f, 0.0f );
		gmtl::Vec3f z_dir ( 0.0f, 0.0f, 1.0f );

		float MoveAloneZ = CorrectAnalogData(mAnologMoveAlongZ);
		float RotateWithY = CorrectAnalogData(mAnologRotateWithY);
		float LookUpDown = CorrectAnalogData(mAnologLookUpDown);

		// Move forward
		if(MoveAloneZ > 0.0f)
			moveBackward();

		if(MoveAloneZ < 0.0f)
			moveForward();

		if(RotateWithY > 0.0f)
			turnRight();

		if(RotateWithY < 0.0f)
			turnLeft();

		if(LookUpDown > 0.0f)
			lookUp();

		if(LookUpDown < 0.0f)
			lookDown();

		if(mDigitalMoveDown->getData() == gadget::Digital::ON)
			moveDown();

		if(mDigitalMoveUp->getData() == gadget::Digital::ON)
			moveUp();

		if(mDigitalExit->getData() == gadget::Digital::TOGGLE_ON)
			exit();

		if(mDigitalInfo->getData() == gadget::Digital::ON){

		}

	}

	float
	GamepadNavigation::CorrectAnalogData(gadget::AnalogInterface AnalogIn){
		float valOut;
		float valIn = AnalogIn->getData();

		if(!mHasData && AnalogIn->getData() > 0.0f){
			mHasData = true;
		}

		if(mHasData){
			valOut = (valIn*2) - 1;

			if(valOut < 0.25 && valOut > -0.25){
				valOut = 0.0f;
			}
			return valOut;
		}else{
			return 0.0f;
		}
	}
}
