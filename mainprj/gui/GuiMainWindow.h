#pragma once
#include "common.h"
#include "GuiContainer.h"
#include "GuiHelper.h"
#include "d3d11imageRender.h"

#include <algorithm>
#include <memory>

namespace My::Gui
{
	using namespace My::Common;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	class MainWindow : public IRenderable
	{
		std::string m_name{ "Main" };
		bool m_visible{};

		GuiContainer* const m_ContainerPtr{};
		IStreamData* const m_data{};
		IStreamControl* const m_control{};

		RenderableTextCombo m_videoSource{"##source", nullptr};

		void initialPosition();

		StringList m_streams;

		void loadData();

	public:

		MainWindow(GuiContainer* const containerPtr, IStreamData* const data, IStreamControl* const control);

		bool render() override;

	};

}

