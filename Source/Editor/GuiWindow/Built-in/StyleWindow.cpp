#include "StyleWindow.h"

#include <imgui.h>

#include <nlohmann/json.hpp>
#include <Engine/Platform/API/PlatformAPI.h>
#include <Engine/Platform/PlatformFile.h>

#include <Engine/Math/ColorConverter.h>

namespace Hollow
{
	StyleWindow::StyleWindow()
	{
		mWindowName = "Style Editor";
	}

	void StyleWindow::OnInitialize()
	{
	}

	void StyleWindow::OnUpdate()
	{
	}

	void StyleWindow::OnPaint()
	{
		ImGui::ShowStyleEditor();

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
		static bool open = true;
		ImGui::Begin("Save Style", &open, flags);
		// Set the window size
		ImGui::SetWindowSize("Save Style", ImVec2(200, 100));
		if (ImGui::Button("Save"))
		{
			ImGuiStyle& style = ImGui::GetStyle();

			String path = PlatformAPI::GetAPI()->GetEngineSourcePath() + "Config/DefaultEditor.json";
			String json;

			PlatformFile::Read(path, json);

			nlohmann::json j;

			j["FrameStyle"]["FrameRounding"] = style.FrameRounding;
			j["FrameStyle"]["WindowRounding"] = style.WindowRounding;
			j["FrameStyle"]["ScrollbarRounding"] = style.ScrollbarRounding;
			j["FrameStyle"]["ScrollbarSize"] = style.ScrollbarSize;
			j["FrameStyle"]["WindowBorderSize"] = style.WindowBorderSize;
			j["FrameStyle"]["ChildBorderSize"] = style.ChildBorderSize;
			j["FrameStyle"]["PopupBorderSize"] = style.PopupBorderSize;
			j["FrameStyle"]["FrameBorderSize"] = style.TabBarBorderSize;
			j["FrameStyle"]["TabBorderSize"] = style.TabBarOverlineSize;
			j["FrameStyle"]["TabRounding"] = style.TabRounding;
			j["FrameStyle"]["TabBorderSize"] = style.TabBorderSize;

			j["ColorStyle"]["TextActive"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_Text].x, 
				style.Colors[ImGuiCol_Text].y, style.Colors[ImGuiCol_Text].z, style.Colors[ImGuiCol_Text].w));
			j["ColorStyle"]["TextDisabled"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_TextDisabled].x, 
				style.Colors[ImGuiCol_TextDisabled].y, style.Colors[ImGuiCol_TextDisabled].z, style.Colors[ImGuiCol_TextDisabled].w));
			j["ColorStyle"]["TextSelectedBg"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_TextSelectedBg].x, 
				style.Colors[ImGuiCol_TextSelectedBg].y, style.Colors[ImGuiCol_TextSelectedBg].z, style.Colors[ImGuiCol_TextSelectedBg].w));
			j["ColorStyle"]["WindowBg"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_WindowBg].x,
				style.Colors[ImGuiCol_WindowBg].y, style.Colors[ImGuiCol_WindowBg].z, style.Colors[ImGuiCol_WindowBg].w));
			j["ColorStyle"]["FrameBg"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_FrameBg].x,
				style.Colors[ImGuiCol_FrameBg].y, style.Colors[ImGuiCol_FrameBg].z, style.Colors[ImGuiCol_FrameBg].w));
			j["ColorStyle"]["FrameBgHovered"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_FrameBgHovered].x,
				style.Colors[ImGuiCol_FrameBgHovered].y, style.Colors[ImGuiCol_FrameBgHovered].z, style.Colors[ImGuiCol_FrameBgHovered].w));
			j["ColorStyle"]["FrameBgActive"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_FrameBgActive].x,
				style.Colors[ImGuiCol_FrameBgActive].y, style.Colors[ImGuiCol_FrameBgActive].z, style.Colors[ImGuiCol_FrameBgActive].w));
			j["ColorStyle"]["TitleBg"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_TitleBg].x,
				style.Colors[ImGuiCol_TitleBg].y, style.Colors[ImGuiCol_TitleBg].z, style.Colors[ImGuiCol_TitleBg].w));
			j["ColorStyle"]["TitleBgActive"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_TitleBgActive].x,
				style.Colors[ImGuiCol_TitleBgActive].y, style.Colors[ImGuiCol_TitleBgActive].z, style.Colors[ImGuiCol_TitleBgActive].w));
			j["ColorStyle"]["TitleBgCollapsed"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_TitleBgCollapsed].x,
				style.Colors[ImGuiCol_TitleBgCollapsed].y, style.Colors[ImGuiCol_TitleBgCollapsed].z, style.Colors[ImGuiCol_TitleBgCollapsed].w));
			j["ColorStyle"]["MenuBarBg"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_MenuBarBg].x,
				style.Colors[ImGuiCol_MenuBarBg].y, style.Colors[ImGuiCol_MenuBarBg].z, style.Colors[ImGuiCol_MenuBarBg].w));
			j["ColorStyle"]["ScrollbarBg"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_ScrollbarBg].x,
				style.Colors[ImGuiCol_ScrollbarBg].y, style.Colors[ImGuiCol_ScrollbarBg].z, style.Colors[ImGuiCol_ScrollbarBg].w));
			j["ColorStyle"]["ScrollbarGrab"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_ScrollbarGrab].x,
				style.Colors[ImGuiCol_ScrollbarGrab].y, style.Colors[ImGuiCol_ScrollbarGrab].z, style.Colors[ImGuiCol_ScrollbarGrab].w));
			j["ColorStyle"]["ScrollbarGrabHovered"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_ScrollbarGrabHovered].x,
				style.Colors[ImGuiCol_ScrollbarGrabHovered].y, style.Colors[ImGuiCol_ScrollbarGrabHovered].z, style.Colors[ImGuiCol_ScrollbarGrabHovered].w));
			j["ColorStyle"]["ScrollbarGrabActive"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_ScrollbarGrabActive].x,
				style.Colors[ImGuiCol_ScrollbarGrabActive].y, style.Colors[ImGuiCol_ScrollbarGrabActive].z, style.Colors[ImGuiCol_ScrollbarGrabActive].w));
			j["ColorStyle"]["CheckMark"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_CheckMark].x,
				style.Colors[ImGuiCol_CheckMark].y, style.Colors[ImGuiCol_CheckMark].z, style.Colors[ImGuiCol_CheckMark].w));
			j["ColorStyle"]["SliderGrab"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_SliderGrab].x,
				style.Colors[ImGuiCol_SliderGrab].y, style.Colors[ImGuiCol_SliderGrab].z, style.Colors[ImGuiCol_SliderGrab].w));
			j["ColorStyle"]["SliderGrabActive"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_SliderGrabActive].x,
				style.Colors[ImGuiCol_SliderGrabActive].y, style.Colors[ImGuiCol_SliderGrabActive].z, style.Colors[ImGuiCol_SliderGrabActive].w));
			j["ColorStyle"]["Button"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_Button].x,
				style.Colors[ImGuiCol_Button].y, style.Colors[ImGuiCol_Button].z, style.Colors[ImGuiCol_Button].w));
			j["ColorStyle"]["ButtonHovered"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_ButtonHovered].x,
				style.Colors[ImGuiCol_ButtonHovered].y, style.Colors[ImGuiCol_ButtonHovered].z, style.Colors[ImGuiCol_ButtonHovered].w));
			j["ColorStyle"]["ButtonActive"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_ButtonActive].x,
				style.Colors[ImGuiCol_ButtonActive].y, style.Colors[ImGuiCol_ButtonActive].z, style.Colors[ImGuiCol_ButtonActive].w));
			j["ColorStyle"]["Header"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_Header].x,
				style.Colors[ImGuiCol_Header].y, style.Colors[ImGuiCol_Header].z, style.Colors[ImGuiCol_Header].w));
			j["ColorStyle"]["HeaderHovered"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_HeaderHovered].x,
				style.Colors[ImGuiCol_HeaderHovered].y, style.Colors[ImGuiCol_HeaderHovered].z, style.Colors[ImGuiCol_HeaderHovered].w));
			j["ColorStyle"]["HeaderActive"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_HeaderActive].x,
				style.Colors[ImGuiCol_HeaderActive].y, style.Colors[ImGuiCol_HeaderActive].z, style.Colors[ImGuiCol_HeaderActive].w));
			j["ColorStyle"]["Separator"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_Separator].x,
				style.Colors[ImGuiCol_Separator].y, style.Colors[ImGuiCol_Separator].z, style.Colors[ImGuiCol_Separator].w));
			j["ColorStyle"]["SeparatorHovered"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_SeparatorHovered].x,
				style.Colors[ImGuiCol_SeparatorHovered].y, style.Colors[ImGuiCol_SeparatorHovered].z, style.Colors[ImGuiCol_SeparatorHovered].w));
			j["ColorStyle"]["SeparatorActive"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_SeparatorActive].x,
				style.Colors[ImGuiCol_SeparatorActive].y, style.Colors[ImGuiCol_SeparatorActive].z, style.Colors[ImGuiCol_SeparatorActive].w));
			j["ColorStyle"]["ResizeGrip"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_ResizeGrip].x,
				style.Colors[ImGuiCol_ResizeGrip].y, style.Colors[ImGuiCol_ResizeGrip].z, style.Colors[ImGuiCol_ResizeGrip].w));
			j["ColorStyle"]["ResizeGripHovered"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_ResizeGripHovered].x,
				style.Colors[ImGuiCol_ResizeGripHovered].y, style.Colors[ImGuiCol_ResizeGripHovered].z, style.Colors[ImGuiCol_ResizeGripHovered].w));
			j["ColorStyle"]["ResizeGripActive"] = Color::RGB_FToHex(Vec4f(style.Colors[ImGuiCol_ResizeGripActive].x,
				style.Colors[ImGuiCol_ResizeGripActive].y, style.Colors[ImGuiCol_ResizeGripActive].z, style.Colors[ImGuiCol_ResizeGripActive].w));

			json = j.dump(4);

			CORE_ASSERT(PlatformFile::Write(path, json), "Editor Style Window", "Failed to write to file");

		}
		ImGui::End();
	}

	void StyleWindow::OnClose()
	{
	}
}