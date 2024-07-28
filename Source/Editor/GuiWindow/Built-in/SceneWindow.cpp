#include "SceneWindow.h"

#include <imgui.h>

namespace Hollow
{
	SceneWindow::SceneWindow()
	{
		mWindowName = "Scene";
	}

	void SceneWindow::OnInitialize()
	{
		ImGui::SetWindowSize(mWindowName.c_str(), ImVec2(500, 500));
	}

	void SceneWindow::OnUpdate()
	{
	}

	void SceneWindow::OnPaint()
	{
		ImGui::Begin(mWindowName.c_str());
		ImGui::Text("Scene");
		ImGui::End();
	}

	void SceneWindow::OnClose()
	{
	}
}
