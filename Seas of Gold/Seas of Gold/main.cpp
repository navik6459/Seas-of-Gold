#include <irrlicht.h>
#include <iostream>
#include "WorldObject.h"
#include "Graphics.h"
#include "UI.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

IrrlichtDevice* loadGRender()
{
	
	//
	IrrlichtDevice *device =
		createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(800, 600), 16,
		false, false, false, 0);

	if (!device)
		return nullptr;
	return device;
}

int main()
{
	IrrlichtDevice* device = loadGRender();
	
	device->setWindowCaption(L"Irrlicht Engine - Sea of Gold");
	device->setResizable(true);
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	//guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
	//	rect<s32>(10, 10, 260, 22), true);
	//Set our font below to built in
	IGUISkin* skin = guienv->getSkin();
	skin->setFont(guienv->getBuiltInFont(), EGDF_TOOLTIP);
	//make some buttons
	guienv->addButton(rect<s32>(10, 240, 110, 240 + 32), 0, GUI_ID_QUIT_BUTTON,
		L"Quit", L"Exits Program");
	guienv->addButton(rect<s32>(10, 280, 110, 280 + 32), 0, GUI_ID_NEW_WINDOW_BUTTON,
		L"New Window", L"Launches a new Window");
	guienv->addButton(rect<s32>(10, 320, 110, 320 + 32), 0, GUI_ID_FILE_OPEN_BUTTON,
		L"File Open", L"Opens a file");
	//This below group of text we create our scrollbar this example links with our alpha values which in turn against the transparency of our GUI.
	guienv->addStaticText(L"Transparent Control:", rect<s32>(150, 20, 350, 40), true);
	IGUIScrollBar* scrollbar = guienv->addScrollBar(true,
		rect<s32>(150, 45, 350, 60), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
	scrollbar->setMax(255);
	scrollbar->setPos(255);
	setSkinTransparency(scrollbar->getPos(), guienv->getSkin());

	// set scrollbar position to alpha value of an arbitrary element
	scrollbar->setPos(guienv->getSkin()->getColor(EGDC_WINDOW).getAlpha());

	guienv->addStaticText(L"Logging ListBox:", rect<s32>(50, 110, 250, 130), true);
	IGUIListBox * listbox = guienv->addListBox(rect<s32>(50, 140, 250, 210));
	guienv->addEditBox(L"Editable Text", rect<s32>(350, 80, 550, 100));

	// Store the appropriate data in a context structure.
	SAppContext context;
	context.device = device;
	context.counter = 0;
	context.listbox = listbox;

	// Then create the event receiver, giving it that context structure.
	MyEventReceiver receiver(context);

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(&receiver);

	IAnimatedMesh* mesh = smgr->getMesh("sydney.md2");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);

	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture(0, driver->getTexture("sydney.bmp"));
	}
	smgr->addCameraSceneNode(0, vector3df(0, 30, -40), vector3df(0, 5, 0));

	//
	driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
	driver->getMaterial2D().AntiAliasing = video::EAAM_FULL_BASIC;

	
	while (device->run())
	{

		driver->beginScene(true, true, SColor(255, 100, 101, 140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}
