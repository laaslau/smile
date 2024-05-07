#include <iostream>
#include "Toolbox.h"
#include "YPLogger.h"
#include "GuiContainer.h"
#include "GuiMainWindow.h"
#include "GuiVideoWindow.h"
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

	container.add("main", &mainW);
	container.add("video", &videoW);

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


