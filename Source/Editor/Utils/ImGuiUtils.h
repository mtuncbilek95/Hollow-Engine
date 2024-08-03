#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Platform/API/PlatformAPI.h>
#include <Engine/Platform/PlatformFile.h>

#include <imgui.h>
#include <nlohmann/json.hpp>

namespace Hollow
{
	namespace ImGuiUtils
	{
		static void DrawVec2f(const String& label, Vec2f& vec, f32 min, f32 max)
		{
			ImGui::DragFloat2(label.c_str(), &vec.x, 0.1f, min, max);
		}

		static void LoadStyleLayout()
		{
			String path = PlatformAPI::GetAPI()->GetEngineSourcePath() + "Config/DefaultEditor.json";

			String binary;
			if (!PlatformFile::Read(path, binary))
			{
				CORE_LOG(HE_ERROR, "Failed to read file [%s].", path.c_str());
				return;
			}

			nlohmann::json json;
			try
			{
				json = nlohmann::json::parse(binary);
			}
			catch (const std::exception& e)
			{
				CORE_LOG(HE_ERROR, "Failed to parse json file [%s].", path.c_str());
				return;
			}

			ImGuiStyle& style = ImGui::GetStyle();

			style.FrameRounding = json["Frame"]["FrameRounding"];
			style.WindowRounding = json["Frame"]["WindowRounding"];
			style.ScrollbarRounding = json["Frame"]["ScrollbarRounding"];
			style.TabRounding = json["Frame"]["TabRounding"];

			style.Colors[ImGuiCol_Text] = ImVec4(json["Colors"]["TextActive"][0], json["Colors"]["TextActive"][1],
				json["Colors"]["TextActive"][2], json["Colors"]["TextActive"][3]);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(json["Colors"]["TextDisabled"][0], json["Colors"]["TextDisabled"][1],
				json["Colors"]["TextDisabled"][2], json["Colors"]["TextDisabled"][3]);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(json["Colors"]["WindowBack"][0], json["Colors"]["WindowBack"][1],
				json["Colors"]["WindowBack"][2], json["Colors"]["WindowBack"][3]);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(json["Colors"]["PopupBack"][0], json["Colors"]["PopupBack"][1],
				json["Colors"]["PopupBack"][2], json["Colors"]["PopupBack"][3]);
			style.Colors[ImGuiCol_Border] = ImVec4(json["Colors"]["Border"][0], json["Colors"]["Border"][1],
				json["Colors"]["Border"][2], json["Colors"]["Border"][3]);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(json["Colors"]["FrameBack"][0], json["Colors"]["FrameBack"][1],
				json["Colors"]["FrameBack"][2], json["Colors"]["FrameBack"][3]);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(json["Colors"]["FrameBackHovered"][0], json["Colors"]["FrameBackHovered"][1],
				json["Colors"]["FrameBackHovered"][2], json["Colors"]["FrameBackHovered"][3]);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(json["Colors"]["FrameBackActive"][0], json["Colors"]["FrameBackActive"][1],
				json["Colors"]["FrameBackActive"][2], json["Colors"]["FrameBackActive"][3]);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(json["Colors"]["TitleBack"][0], json["Colors"]["TitleBack"][1],
				json["Colors"]["TitleBack"][2], json["Colors"]["TitleBack"][3]);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(json["Colors"]["TitleBackActive"][0], json["Colors"]["TitleBackActive"][1],
				json["Colors"]["TitleBackActive"][2], json["Colors"]["TitleBackActive"][3]);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(json["Colors"]["MenuBarBack"][0], json["Colors"]["MenuBarBack"][1],
				json["Colors"]["MenuBarBack"][2], json["Colors"]["MenuBarBack"][3]);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(json["Colors"]["MenuBar"][0], json["Colors"]["MenuBar"][1],
				json["Colors"]["MenuBar"][2], json["Colors"]["MenuBar"][3]);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(json["Colors"]["ScrollbarGrab"][0], json["Colors"]["ScrollbarGrab"][1],
				json["Colors"]["ScrollbarGrab"][2], json["Colors"]["ScrollbarGrab"][3]);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(json["Colors"]["ScrollbarGrabHovered"][0], json["Colors"]["ScrollbarGrabHovered"][1],
				json["Colors"]["ScrollbarGrabHovered"][2], json["Colors"]["ScrollbarGrabHovered"][3]);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(json["Colors"]["ScrollbarGrabActive"][0], json["Colors"]["ScrollbarGrabActive"][1],
				json["Colors"]["ScrollbarGrabActive"][2], json["Colors"]["ScrollbarGrabActive"][3]);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(json["Colors"]["CheckMark"][0], json["Colors"]["CheckMark"][1],
				json["Colors"]["CheckMark"][2], json["Colors"]["CheckMark"][3]);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(json["Colors"]["SliderGrab"][0], json["Colors"]["SliderGrab"][1],
				json["Colors"]["SliderGrab"][2], json["Colors"]["SliderGrab"][3]);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(json["Colors"]["SliderGrabActive"][0], json["Colors"]["SliderGrabActive"][1],
				json["Colors"]["SliderGrabActive"][2], json["Colors"]["SliderGrabActive"][3]);
			style.Colors[ImGuiCol_Button] = ImVec4(json["Colors"]["Button"][0], json["Colors"]["Button"][1],
				json["Colors"]["Button"][2], json["Colors"]["Button"][3]);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(json["Colors"]["ButtonHovered"][0], json["Colors"]["ButtonHovered"][1],
				json["Colors"]["ButtonHovered"][2], json["Colors"]["ButtonHovered"][3]);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(json["Colors"]["ButtonActive"][0], json["Colors"]["ButtonActive"][1],
				json["Colors"]["ButtonActive"][2], json["Colors"]["ButtonActive"][3]);
			style.Colors[ImGuiCol_Header] = ImVec4(json["Colors"]["Header"][0], json["Colors"]["Header"][1],
				json["Colors"]["Header"][2], json["Colors"]["Header"][3]);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(json["Colors"]["HeaderHovered"][0], json["Colors"]["HeaderHovered"][1],
				json["Colors"]["HeaderHovered"][2], json["Colors"]["HeaderHovered"][3]);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(json["Colors"]["HeaderActive"][0], json["Colors"]["HeaderActive"][1],
				json["Colors"]["HeaderActive"][2], json["Colors"]["HeaderActive"][3]);
			style.Colors[ImGuiCol_Separator] = ImVec4(json["Colors"]["Separator"][0], json["Colors"]["Separator"][1],
				json["Colors"]["Separator"][2], json["Colors"]["Separator"][3]);
			style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(json["Colors"]["SeparatorHovered"][0], json["Colors"]["SeparatorHovered"][1],
				json["Colors"]["SeparatorHovered"][2], json["Colors"]["SeparatorHovered"][3]);
			style.Colors[ImGuiCol_SeparatorActive] = ImVec4(json["Colors"]["SeparatorActive"][0], json["Colors"]["SeparatorActive"][1],
				json["Colors"]["SeparatorActive"][2], json["Colors"]["SeparatorActive"][3]);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(json["Colors"]["ResizeGrip"][0], json["Colors"]["ResizeGrip"][1],
				json["Colors"]["ResizeGrip"][2], json["Colors"]["ResizeGrip"][3]);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(json["Colors"]["ResizeGripHovered"][0], json["Colors"]["ResizeGripHovered"][1],
				json["Colors"]["ResizeGripHovered"][2], json["Colors"]["ResizeGripHovered"][3]);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(json["Colors"]["ResizeGripActive"][0], json["Colors"]["ResizeGripActive"][1],
				json["Colors"]["ResizeGripActive"][2], json["Colors"]["ResizeGripActive"][3]);
			style.Colors[ImGuiCol_Tab] = ImVec4(json["Colors"]["Tab"][0], json["Colors"]["Tab"][1],
				json["Colors"]["Tab"][2], json["Colors"]["Tab"][3]);
			style.Colors[ImGuiCol_TabHovered] = ImVec4(json["Colors"]["TabHovered"][0], json["Colors"]["TabHovered"][1],
				json["Colors"]["TabHovered"][2], json["Colors"]["TabHovered"][3]);
			style.Colors[ImGuiCol_TabActive] = ImVec4(json["Colors"]["TabActive"][0], json["Colors"]["TabActive"][1],
				json["Colors"]["TabActive"][2], json["Colors"]["TabActive"][3]);
			style.Colors[ImGuiCol_TabUnfocused] = ImVec4(json["Colors"]["TabUnfocused"][0], json["Colors"]["TabUnfocused"][1],
				json["Colors"]["TabUnfocused"][2], json["Colors"]["TabUnfocused"][3]);
			style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(json["Colors"]["TabUnfocusedActive"][0], json["Colors"]["TabUnfocusedActive"][1],
				json["Colors"]["TabUnfocusedActive"][2], json["Colors"]["TabUnfocusedActive"][3]);
			style.Colors[ImGuiCol_TabDimmedSelectedOverline] = { 0,0,0,0 };
			style.Colors[ImGuiCol_TabSelectedOverline] = { 0,0,0,0 };
		}

		static void SaveStyleLayout()
		{
			nlohmann::json json;

			ImGuiStyle& style = ImGui::GetStyle();

			json["Frame"]["FrameRounding"] = style.FrameRounding;
			json["Frame"]["WindowRounding"] = style.WindowRounding;
			json["Frame"]["ScrollbarRounding"] = style.ScrollbarRounding;
			json["Frame"]["TabRounding"] = style.TabRounding;

			json["Colors"]["TextActive"] = { style.Colors[ImGuiCol_Text].x, style.Colors[ImGuiCol_Text].y,
				style.Colors[ImGuiCol_Text].z, style.Colors[ImGuiCol_Text].w };
			json["Colors"]["TextDisabled"] = { style.Colors[ImGuiCol_TextDisabled].x, style.Colors[ImGuiCol_TextDisabled].y,
				style.Colors[ImGuiCol_TextDisabled].z, style.Colors[ImGuiCol_TextDisabled].w };
			json["Colors"]["WindowBack"] = { style.Colors[ImGuiCol_WindowBg].x, style.Colors[ImGuiCol_WindowBg].y,
				style.Colors[ImGuiCol_WindowBg].z, style.Colors[ImGuiCol_WindowBg].w };
			json["Colors"]["ChildBack"] = json["Colors"]["WindowBack"];
			json["Colors"]["PopupBack"] = { style.Colors[ImGuiCol_PopupBg].x, style.Colors[ImGuiCol_PopupBg].y,
				style.Colors[ImGuiCol_PopupBg].z, style.Colors[ImGuiCol_PopupBg].w };
			json["Colors"]["Border"] = { style.Colors[ImGuiCol_Border].x, style.Colors[ImGuiCol_Border].y,
				style.Colors[ImGuiCol_Border].z, style.Colors[ImGuiCol_Border].w };
			json["Colors"]["FrameBack"] = { style.Colors[ImGuiCol_FrameBg].x, style.Colors[ImGuiCol_FrameBg].y,
				style.Colors[ImGuiCol_FrameBg].z, style.Colors[ImGuiCol_FrameBg].w };
			json["Colors"]["FrameBackHovered"] = { style.Colors[ImGuiCol_FrameBgHovered].x, style.Colors[ImGuiCol_FrameBgHovered].y,
				style.Colors[ImGuiCol_FrameBgHovered].z, style.Colors[ImGuiCol_FrameBgHovered].w };
			json["Colors"]["FrameBackActive"] = { style.Colors[ImGuiCol_FrameBgActive].x, style.Colors[ImGuiCol_FrameBgActive].y,
				style.Colors[ImGuiCol_FrameBgActive].z, style.Colors[ImGuiCol_FrameBgActive].w };
			json["Colors"]["TitleBack"] = { style.Colors[ImGuiCol_TitleBg].x, style.Colors[ImGuiCol_TitleBg].y,
				style.Colors[ImGuiCol_TitleBg].z, 0 };
			json["Colors"]["TitleBackActive"] = { style.Colors[ImGuiCol_TitleBgActive].x, style.Colors[ImGuiCol_TitleBgActive].y,
				style.Colors[ImGuiCol_TitleBgActive].z, style.Colors[ImGuiCol_TitleBgActive].w };
			json["Colors"]["TitleBackCollapsed"] = { 0, 0, 0, 0 };
			json["Colors"]["MenuBarBack"] = { style.Colors[ImGuiCol_MenuBarBg].x, style.Colors[ImGuiCol_MenuBarBg].y,
				style.Colors[ImGuiCol_MenuBarBg].z, style.Colors[ImGuiCol_MenuBarBg].w };
			json["Colors"]["MenuBar"] = { style.Colors[ImGuiCol_ScrollbarBg].x, style.Colors[ImGuiCol_ScrollbarBg].y,
				style.Colors[ImGuiCol_ScrollbarBg].z, style.Colors[ImGuiCol_ScrollbarBg].w };
			json["Colors"]["ScrollbarBack"] = { style.Colors[ImGuiCol_ScrollbarBg].x, style.Colors[ImGuiCol_ScrollbarBg].y,
				style.Colors[ImGuiCol_ScrollbarBg].z, style.Colors[ImGuiCol_ScrollbarBg].w };
			json["Colors"]["ScrollbarGrab"] = { style.Colors[ImGuiCol_ScrollbarGrab].x, style.Colors[ImGuiCol_ScrollbarGrab].y,
				style.Colors[ImGuiCol_ScrollbarGrab].z, style.Colors[ImGuiCol_ScrollbarGrab].w };
			json["Colors"]["ScrollbarGrabHovered"] = { style.Colors[ImGuiCol_ScrollbarGrabHovered].x, style.Colors[ImGuiCol_ScrollbarGrabHovered].y,
				style.Colors[ImGuiCol_ScrollbarGrabHovered].z, style.Colors[ImGuiCol_ScrollbarGrabHovered].w };
			json["Colors"]["ScrollbarGrabActive"] = { style.Colors[ImGuiCol_ScrollbarGrabActive].x, style.Colors[ImGuiCol_ScrollbarGrabActive].y,
				style.Colors[ImGuiCol_ScrollbarGrabActive].z, style.Colors[ImGuiCol_ScrollbarGrabActive].w };
			json["Colors"]["CheckMark"] = { style.Colors[ImGuiCol_CheckMark].x, style.Colors[ImGuiCol_CheckMark].y,
				style.Colors[ImGuiCol_CheckMark].z, style.Colors[ImGuiCol_CheckMark].w };
			json["Colors"]["SliderGrab"] = { style.Colors[ImGuiCol_SliderGrab].x, style.Colors[ImGuiCol_SliderGrab].y,
				style.Colors[ImGuiCol_SliderGrab].z, style.Colors[ImGuiCol_SliderGrab].w };
			json["Colors"]["SliderGrabActive"] = { style.Colors[ImGuiCol_SliderGrabActive].x, style.Colors[ImGuiCol_SliderGrabActive].y,
				style.Colors[ImGuiCol_SliderGrabActive].z, style.Colors[ImGuiCol_SliderGrabActive].w };
			json["Colors"]["Button"] = { style.Colors[ImGuiCol_Button].x, style.Colors[ImGuiCol_Button].y,
				style.Colors[ImGuiCol_Button].z, style.Colors[ImGuiCol_Button].w };
			json["Colors"]["ButtonHovered"] = { style.Colors[ImGuiCol_ButtonHovered].x, style.Colors[ImGuiCol_ButtonHovered].y,
				style.Colors[ImGuiCol_ButtonHovered].z, style.Colors[ImGuiCol_ButtonHovered].w };
			json["Colors"]["ButtonActive"] = { style.Colors[ImGuiCol_ButtonActive].x, style.Colors[ImGuiCol_ButtonActive].y,
				style.Colors[ImGuiCol_ButtonActive].z, style.Colors[ImGuiCol_ButtonActive].w };
			json["Colors"]["Header"] = { style.Colors[ImGuiCol_Header].x, style.Colors[ImGuiCol_Header].y,
				style.Colors[ImGuiCol_Header].z, style.Colors[ImGuiCol_Header].w };
			json["Colors"]["HeaderHovered"] = { style.Colors[ImGuiCol_HeaderHovered].x, style.Colors[ImGuiCol_HeaderHovered].y,
				style.Colors[ImGuiCol_HeaderHovered].z, style.Colors[ImGuiCol_HeaderHovered].w };
			json["Colors"]["HeaderActive"] = { style.Colors[ImGuiCol_HeaderActive].x, style.Colors[ImGuiCol_HeaderActive].y,
				style.Colors[ImGuiCol_HeaderActive].z, style.Colors[ImGuiCol_HeaderActive].w };
			json["Colors"]["Separator"] = { style.Colors[ImGuiCol_Separator].x, style.Colors[ImGuiCol_Separator].y,
				style.Colors[ImGuiCol_Separator].z, style.Colors[ImGuiCol_Separator].w };
			json["Colors"]["SeparatorHovered"] = { style.Colors[ImGuiCol_SeparatorHovered].x, style.Colors[ImGuiCol_SeparatorHovered].y,
				style.Colors[ImGuiCol_SeparatorHovered].z, style.Colors[ImGuiCol_SeparatorHovered].w };
			json["Colors"]["SeparatorActive"] = { style.Colors[ImGuiCol_SeparatorActive].x, style.Colors[ImGuiCol_SeparatorActive].y,
				style.Colors[ImGuiCol_SeparatorActive].z, style.Colors[ImGuiCol_SeparatorActive].w };
			json["Colors"]["ResizeGrip"] = { style.Colors[ImGuiCol_ResizeGrip].x, style.Colors[ImGuiCol_ResizeGrip].y,
				style.Colors[ImGuiCol_ResizeGrip].z, style.Colors[ImGuiCol_ResizeGrip].w };
			json["Colors"]["ResizeGripHovered"] = { style.Colors[ImGuiCol_ResizeGripHovered].x, style.Colors[ImGuiCol_ResizeGripHovered].y,
				style.Colors[ImGuiCol_ResizeGripHovered].z, style.Colors[ImGuiCol_ResizeGripHovered].w };
			json["Colors"]["ResizeGripActive"] = { style.Colors[ImGuiCol_ResizeGripActive].x, style.Colors[ImGuiCol_ResizeGripActive].y,
				style.Colors[ImGuiCol_ResizeGripActive].z, style.Colors[ImGuiCol_ResizeGripActive].w };
			json["Colors"]["Tab"] = { style.Colors[ImGuiCol_Tab].x, style.Colors[ImGuiCol_Tab].y,
				style.Colors[ImGuiCol_Tab].z, style.Colors[ImGuiCol_Tab].w };
			json["Colors"]["TabHovered"] = { style.Colors[ImGuiCol_TabHovered].x, style.Colors[ImGuiCol_TabHovered].y,
				style.Colors[ImGuiCol_TabHovered].z, style.Colors[ImGuiCol_TabHovered].w };
			json["Colors"]["TabActive"] = { style.Colors[ImGuiCol_TabActive].x, style.Colors[ImGuiCol_TabActive].y,
				style.Colors[ImGuiCol_TabActive].z, style.Colors[ImGuiCol_TabActive].w };
			json["Colors"]["TabUnfocused"] = { style.Colors[ImGuiCol_TabUnfocused].x, style.Colors[ImGuiCol_TabUnfocused].y,
				style.Colors[ImGuiCol_TabUnfocused].z, style.Colors[ImGuiCol_TabUnfocused].w };
			json["Colors"]["TabUnfocusedActive"] = { style.Colors[ImGuiCol_TabUnfocusedActive].x, style.Colors[ImGuiCol_TabUnfocusedActive].y,
				style.Colors[ImGuiCol_TabUnfocusedActive].z, style.Colors[ImGuiCol_TabUnfocusedActive].w };
			json["Colors"]["TabDimOverline"] = { 0, 0, 0, 0 };
			json["Colors"]["TabSelectedOverline"] = { 0, 0, 0, 0 };

			String path = PlatformAPI::GetAPI()->GetEngineSourcePath() + "Config/DefaultEditor.json";

			String binary = json.dump(4);

			if (!PlatformFile::Write(path, binary))
				CORE_LOG(HE_ERROR, "Failed to write file [%s].", path.c_str());
		}
	}
}
