#include "PropertyWindow.h"

#include <imgui.h>

namespace Hollow
{
	PropertyWindow::PropertyWindow()
	{
		mWindowName = "Entity Properties";
	}

	void PropertyWindow::OnInitialize()
	{
		ImGui::SetWindowSize(mWindowName.c_str(), ImVec2(200, 200));
	}

	void PropertyWindow::OnUpdate()
	{
	}

	void PropertyWindow::OnPaint()
	{
		ImGui::Begin(mWindowName.c_str());
		ImGui::Text("Entity Properties");
		ImGui::End();
	}

	void PropertyWindow::OnClose()
	{
	}
}
