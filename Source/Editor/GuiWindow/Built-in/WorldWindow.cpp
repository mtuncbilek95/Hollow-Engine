#include "WorldWindow.h"

#include <imgui.h>

namespace Hollow
{
	WorldWindow::WorldWindow()
	{
		mWindowName = "World Details";
	}

	void WorldWindow::OnInitialize()
	{
		ImGui::SetWindowSize(mWindowName.c_str(), ImVec2(200, 200));
	}

	void WorldWindow::OnUpdate()
	{
	}

	void WorldWindow::OnPaint()
	{
		ImGui::Begin(mWindowName.c_str());
		ImGui::Text("World Details");
		ImGui::End();
	}

	void WorldWindow::OnClose()
	{
	}
}
