#include <iostream>
#include "Toolbox.h"
#include "YPLogger.h"
#include "GuiContainer.h"
#include "GuiMainWindow.h"
#include "GuiVideoWindow.h"
#include "GuiResultWindow.h"
#include "Factory.h"


static int mine()
{
	
	const auto& pth = My::Toolbox::exePathA().string();
	My::Logger::Log log(pth, "");
	plog::init(plog::debug, &log);

	PLOGI << "--------------------------------------------";
	PLOGI << pth << " started.";

	My::Gui::GuiContainer container("cvbench", "1.0.0.0");

	My::Gui::MainWindow mainW(&container, My::CvLib::getStreamData(), My::CvLib::getStreamControl());
	My::Gui::VideoWindow videoW("Video", My::CvLib::getVideoSource());
	My::Gui::VideoWindow videoS("Smile", My::CvLib::getSmiledSource());
	My::Gui::ResultWindow videoR(My::CvLib::getStreamData(), My::CvLib::getSmilePictogram());

	container.add("main", &mainW);
	container.add("video", &videoW);
	container.add("smile", &videoS);
	container.add("result", &videoR);

	while (container.render()) {}
	
	PLOGI << pth << " exits.";
	PLOGI << "--------------------------------------------";
	
	return 0;
}

int main()
{
	return mine();
}

int APIENTRY wWinMain(_In_ HINSTANCE /*hInstance*/,
	_In_opt_ HINSTANCE /*hPrevInstance*/,
	_In_ LPWSTR    /*lpCmdLine*/,
	_In_ int       /*nCmdShow*/)
{
	return mine();

}


