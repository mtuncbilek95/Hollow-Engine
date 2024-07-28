#include "ContentWindow.h"

#include <imgui.h>

namespace Hollow
{
	ContentWindow::ContentWindow()
	{
		mWindowName = "Content Browser";
	}

	void ContentWindow::OnInitialize()
	{
		ImGui::SetWindowSize(mWindowName.c_str(), ImVec2(200, 50));
	}

	void ContentWindow::OnUpdate()
	{
	}

	void ContentWindow::OnPaint()
	{
		ImGui::Begin(mWindowName.c_str());
		ImGui::Text("Content Browser");
		ImGui::End();
	}

	void ContentWindow::OnClose()
	{
	}
}
