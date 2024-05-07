#include "GuiMainWindow.h"
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
	MainWindow::MainWindow(GuiContainer* const containerPtr, IStreamData* const data, IStreamControl* const control) :
		m_ContainerPtr{ containerPtr }, m_data{ data }, m_control{ control }
	{
		loadData();
	}

	void MainWindow::loadData()
	{
		m_streams = m_data->getStreamNames();
		m_videoSource.setComboDataPtr(&m_streams);
	}

	void MainWindow::initialPosition()
	{
		const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
		auto shft = ImGui::GetFrameHeight() * 2;
		ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + shft, main_viewport->WorkPos.y + shft), WIN_APPEARANCE);
		ImGui::SetNextWindowSize(ImVec2(main_viewport->Size.x / 2.f, main_viewport->Size.y / 4.f), WIN_APPEARANCE);
	}

	bool MainWindow::render()
	{
		if (!m_visible)
		{
			initialPosition();
			m_visible = true;

			m_videoSource.setText(m_data->getStreamName());
		}

		ImGui::Begin(m_name.c_str());

		ImGui::BeginDisabled(m_control->isRunning());

		if (ImGui::BeginTable("##tab1", 2, ImGuiTableFlags_SizingStretchProp))// | ImGuiTableFlags_Borders))
		{
			ImGui::TableSetupColumn("##1", ImGuiTableColumnFlags_WidthFixed, ImGui::GetFrameHeight() * 6);
			//--------------------------------------------------------------------------------------------
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Video Source");

			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
			if (m_videoSource.render())
			{
				m_data->setStreamName(m_videoSource.getText());
				loadData();
			}
			
			//--------------------------------------------------------------------------------------------

			ImGui::EndTable();
		}
		ImGui::EndDisabled();

		ImGui::Separator();
		ImGui::BeginDisabled(m_streams.empty());

		bool running = m_control->isRunning();
		if (ImGui::Button(running ? "Stop" : "Start"))
		{
			m_control->run(!running);
		}

		ImGui::EndDisabled();

		ImGui::End();


		return m_visible;
	}



}