#pragma once

#include <Engine/Core/Core.h>

#include <imgui.h>

namespace Hollow
{
	namespace ImGuiUtils
	{
		static void DrawVec2f(const String& label, Vec2f& vec, f32 min, f32 max)
		{
			ImGui::DragFloat2(label.c_str(), &vec.x, 0.1f, min, max);
		}
	}
}
