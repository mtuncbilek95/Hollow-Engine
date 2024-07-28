#include "DockWindow.h"
#include "MenuBar.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Hollow
{
	DockWindow::DockWindow()
	{
		mWindowName = "DockSpaceWindow";
		mPosition = Vec2f(0.0f, 0.0f);
		mWindowSize = Vec2f(200.0f, 200.0f);

		mIsOpen = true;
		mIsDirty = true;

		mMenuBar = MakeShared<MenuBar>();
	}

	void DockWindow::OnInitialize()
	{
	}

	void DockWindow::OnUpdate()
	{
	}

	void DockWindow::OnPaint()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", NULL, window_flags);
		ImGui::PopStyleVar(3);

		mMenuBar->OnPaint();

		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

		if (mIsDirty)
		{
			ImGui::DockBuilderRemoveNode(dockspace_id); // Clear any previous layout
			ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None); // Add empty node
			ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

			ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.20f, nullptr, &dockspace_id);
			ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.20f, nullptr, &dockspace_id);
			ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);

			// Apply flags to each dock node
			ImGuiDockNode* node_left = ImGui::DockBuilderGetNode(dock_id_left);
			if (node_left)
				node_left->LocalFlags |= ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoResize;

			ImGuiDockNode* node_right = ImGui::DockBuilderGetNode(dock_id_right);
			if (node_right)
				node_right->LocalFlags |= ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoResize;

			ImGuiDockNode* node_bottom = ImGui::DockBuilderGetNode(dock_id_bottom);
			if (node_bottom)
				node_bottom->LocalFlags |= ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoResize;

			ImGuiDockNode* node_center = ImGui::DockBuilderGetNode(dockspace_id);
			if (node_center)
				node_center->LocalFlags |= ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoResize;

			// Dock the windows into the respective nodes
			ImGui::DockBuilderDockWindow("World Details", dock_id_left);
			ImGui::DockBuilderDockWindow("Entity Properties", dock_id_right);
			ImGui::DockBuilderDockWindow("Content Browser", dock_id_bottom);
			ImGui::DockBuilderDockWindow("Scene", dockspace_id); // Center

			// Finish the dock builder process
			ImGui::DockBuilderFinish(dockspace_id);

			mIsDirty = false;
		}

		ImGui::End();
	}

	void DockWindow::OnClose()
	{
	}
}
