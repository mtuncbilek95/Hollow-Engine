#include "StyleWindow.h"

#include <imgui.h>

#include <nlohmann/json.hpp>
#include <Engine/Platform/API/PlatformAPI.h>
#include <Engine/Platform/PlatformFile.h>

#include <Engine/Math/ColorConverter.h>
#include <Editor/Utils/ImGuiUtils.h>

#include <Editor/API/ImGuiWindowAPI.h>

namespace Hollow
{
	StyleWindow::StyleWindow()
	{
		mWindowName = "Style Editor";
		mIsOpen = true;
		mIsDirty = false;
	}

	StyleWindow::~StyleWindow()
	{

	}

	void StyleWindow::OnInitialize()
	{
	}

	void StyleWindow::OnUpdate()
	{
	}

	void StyleWindow::OnPaint()
	{
		ImGui::Begin("Editor Style", &mIsOpen);

		ImGui::ShowStyleEditor();

		if (ImGui::Button("Save"))
		{
			ImGuiUtils::SaveStyleLayout();
		}

		ImGui::End();

		if (!mIsOpen)
			ImGuiWindowAPI::GetAPI()->UnregisterGuiWindow(mWindowName);
	}

	void StyleWindow::OnClose()
	{
	}
}