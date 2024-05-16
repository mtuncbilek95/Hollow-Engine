#include "ImGuiRenderer.h"

namespace Hollow
{
	ImGuiRenderer::ImGuiRenderer()
	{
		mDevice = GraphicsManager::GetInstanceAPI().GetDefaultDevice();
	}

	void ImGuiRenderer::CreateImguiResources()
	{
		mContext = ImGui::CreateContext();
		ImGui::StyleColorsDark();



	}

	void ImGuiRenderer::OnShutdown()
	{
	}
}
