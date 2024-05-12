#include "ImGuiRenderer.h"

namespace Hollow
{
	ImGuiRenderer::ImGuiRenderer()
	{
		mContext = ImGui::CreateContext();
		ImGui::StyleColorsDark();


	}

	void ImGuiRenderer::OnShutdown()
	{
	}
}
