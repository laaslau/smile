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


	class ResultWindow : public IRenderable
	{
		std::string m_name{ "Result" };
		bool m_visible{};
		void initialPosition();
		ImageRender m_imageRender;

		My::Common::IVideoSource* m_pic;

	public:

		ResultWindow(My::Common::IVideoSource* pic);

		bool render() override;

	};

}

