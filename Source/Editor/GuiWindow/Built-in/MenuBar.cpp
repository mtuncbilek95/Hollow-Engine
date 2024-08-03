#include "MenuBar.h"

#include <Editor/API/ImGuiWindowAPI.h>
#include <Editor/GuiWindow/Built-in/StyleWindow.h>

#include <imgui.h>

#include <Engine/Math/ColorConverter.h>

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

				}
				ImGui::SameLine();
				ImGui::TextDisabled("(?)");

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::TextUnformatted("Create a new empty level. You can also use");
					Vec4f color = Color::HexToRGB_F("#FED1524");
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.x, color.y, color.z, 1.0f));
					ImGui::TextUnformatted("Ctrl+Alt+N");
					ImGui::PopStyleColor();
					ImGui::SameLine();
					ImGui::TextUnformatted("to create with shortcut");
					ImGui::EndTooltip();
				}

				if (ImGui::MenuItem("Open Level"))
				{
					// Do something
				}
				ImGui::SameLine();
				ImGui::TextDisabled("(?)");

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::TextUnformatted("Open an existing level. You can also use");
					Vec4f color = Color::HexToRGB_F("#FED1524");
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.x, color.y, color.z, 1.0f));
					ImGui::TextUnformatted("Ctrl+Alt+O");
					ImGui::PopStyleColor();
					ImGui::SameLine();
					ImGui::TextUnformatted("to open with shortcut");
					ImGui::EndTooltip();
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
					if (auto newWindow = ImGuiWindowAPI::GetAPI()->GetGuiWindow<StyleWindow>("Style Editor"))
						ImGuiWindowAPI::GetAPI()->UnregisterGuiWindow("Style Editor");
					else
						ImGuiWindowAPI::GetAPI()->RegisterGuiWindow<StyleWindow>();
				}
				ImGui::SameLine();
				ImGui::TextDisabled("(?)");

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::TextUnformatted("Editor's layout configuration panel for\nthose who want to edit the layout\nfor their preferences");
					ImGui::EndTooltip();
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
