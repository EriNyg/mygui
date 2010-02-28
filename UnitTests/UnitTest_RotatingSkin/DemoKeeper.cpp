/*!
	@file
	@author		George Evmenov
	@date		02/2010
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	MyGUI::StaticImage * image;
	MyGUI::RotatingSkin * rotato;

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_Layers");
		addResourceLocation(getRootMedia() + "/Common/Scene");
	}

	void DemoKeeper::createScene()
	{
		getGUI()->eventFrameStart += MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStarted);

		MyGUI::WindowPtr window = getGUI()->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(400, 400, 400, 400), MyGUI::Align::Default, "Main");

		image = window->createWidget<MyGUI::StaticImage>("RotatingSkin", MyGUI::IntCoord(150, 150, 100, 150), MyGUI::Align::Default/*, "Main"*/);
		image->setImageTexture("core.png");

		MyGUI::ISubWidget * main = image->getSubWidgetMain();
		rotato = main->castType<MyGUI::RotatingSkin>();
	}

	void DemoKeeper::destroyScene()
	{
		getGUI()->eventFrameStart -= MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStarted);
	}

	void DemoKeeper::notifyFrameStarted(float _time)
	{
	}

	void DemoKeeper::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Right)
			rotato->setCenter(MyGUI::IntPoint(_absx, _absy) - image->getAbsolutePosition());

		base::BaseManager::injectMousePress(_absx, _absy, _id);
	}

	void DemoKeeper::injectMouseMove(int _absx, int _absy, int _absz)
	{
		rotato->setAngle(atan2((double)_absx - rotato->getCenter(false).left, -(double)_absy + rotato->getCenter(false).top));

		base::BaseManager::injectMouseMove(_absx, _absy, _absz);
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)