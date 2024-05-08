#include "GuiResultWindow.h"
#include "IconsFontAwesome5.h"
#include "fontdata.h"
#include "d3d11imageRender.h"
#include "GuiHelper.h"
#include "imspinner.h"
#include <ranges>

namespace My::Gui
{

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////
	ResultWindow::ResultWindow(My::Common::IStreamData* streamData, My::Common::IVideoSource* pic) : m_pic{ pic }, m_streamData{streamData},
		m_imageRender{ GuiContainer::m_d3d.g_pd3dDevice, pic }
	{
	}

	void ResultWindow::initialPosition()
	{
		const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
		auto shft = ImGui::GetFrameHeight() * 2;
		ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + shft, main_viewport->WorkPos.y + shft), WIN_APPEARANCE);
		ImGui::SetNextWindowSize(ImVec2(main_viewport->Size.x / 2.f, main_viewport->Size.y / 4.f), WIN_APPEARANCE);
	}

	bool ResultWindow::render()
	{
		if (!m_visible)
		{
			initialPosition();
			m_visible = true;
		}

		ImGui::Begin(m_name.c_str());
		const auto& ic = imageCoord();
		if (m_streamData->getFaceDir())
		{
			m_imageRender.render(ic.x, ic.y, ic.z, ic.w);
		}
		else
		{
			ImGui::SetCursorPos({ ic.x, ic.y + ic.w });
		}


		ImGui::Separator();
		auto frameRate = ImGui::GetIO().Framerate;
		ImGui::Text("Render: %.3f ms/frame (%.1f FPS).", 1000.0f / frameRate, frameRate);
		ImGui::End();

		return m_visible;
	}



}