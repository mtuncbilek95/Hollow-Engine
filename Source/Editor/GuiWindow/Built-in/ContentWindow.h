#pragma once

#include <Engine/Core/Core.h>
#include <Editor/GuiWindow/GuiWindow.h>

namespace Hollow
{
	class ContentWindow : public GuiWindow
	{
	public:
		ContentWindow();
		~ContentWindow() override = default;

		virtual void OnInitialize() override;
		virtual void OnUpdate() override;
		virtual void OnPaint() override;
		virtual void OnClose() override;
	};
}
