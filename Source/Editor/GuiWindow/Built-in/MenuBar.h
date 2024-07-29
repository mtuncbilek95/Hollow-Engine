#pragma once

#include <Engine/Core/Core.h>
#include <Editor/GuiWindow/GuiWindow.h>

namespace Hollow
{
	class MenuBar : public GuiWindow
	{
	public:
		MenuBar();
		~MenuBar() override = default;

		virtual void OnInitialize() override;
		virtual void OnUpdate() override;
		virtual void OnPaint() override;
		virtual void OnClose() override;
	};
}
