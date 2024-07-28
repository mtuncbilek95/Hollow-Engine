#include "MenuBar.h"

#include <Editor/API/ImGuiWindowAPI.h>
#include <Editor/GuiWindow/Built-in/StyleWindow.h>

#include <imgui.h>

namespace Hollow
{

	MenuBar::MenuBar()
	{
	}

	void MenuBar::OnInitialize()
	{
	}

	void MenuBar::OnUpdate()
	{
	}

	void MenuBar::OnPaint()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Level"))
				{
					// Do something
				}

				if (ImGui::MenuItem("Open Level"))
				{
					// Do something
				}

				if (ImGui::MenuItem("Save Level"))
				{
					// Do something
				}

				if (ImGui::MenuItem("Exit Editor"))
				{
					ImGuiWindowAPI::GetAPI()->RequestQuit();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo"))
				{
					// Do something
				}

				if (ImGui::MenuItem("Redo"))
				{
					// Do something
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Settings"))
				{
					// Do something
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Editor Style Layout"))
				{
					ImGuiWindowAPI::GetAPI()->RegisterGuiWindow<StyleWindow>();
				}

				ImGui::EndMenu();
			}
		}

		ImGui::EndMenuBar();
	}

	void MenuBar::OnClose()
	{
	}
}
