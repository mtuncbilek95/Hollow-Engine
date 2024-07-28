#include "DockWindow.h"

#include <imgui.h>

namespace Hollow
{
	DockWindow::DockWindow()
	{
		mWindowName = "Dock Window";
		mPosition = Vec2f(0.0f, 0.0f);
		mWindowSize = Vec2f(200.0f, 200.0f);

		mIsOpen = true;
		mIsDirty = false;
		mIsFocused = false;
		mIsHovered = false;
	}

	void DockWindow::OnInitialize()
	{
	}

	void DockWindow::OnUpdate()
	{
	}

	void DockWindow::OnPaint()
	{

		ImGuiViewport* pViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(pViewport->Pos);
		ImGui::SetNextWindowSize(pViewport->Size);
		ImGui::SetNextWindowViewport(pViewport->ID);

		ImGui::Begin("DockSpaceWindow", &mIsOpen,
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus);

		ImGuiID dockspace_id = ImGui::GetID("Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);
		ImGui::End();
	}

	void DockWindow::OnClose()
	{
	}
}
