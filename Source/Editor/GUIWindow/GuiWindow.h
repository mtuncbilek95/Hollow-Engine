#pragma once

#include <Engine/Core/Core.h>
#include <Editor/Object/IEditorObject.h>

namespace Hollow
{
	class EDITOR_API GuiWindow : public IEditorObject
	{
	public:
		virtual ~GuiWindow() = default;

		virtual void OnInitialize() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnPaint() = 0;
		virtual void OnClose() = 0;

	protected:
		String mWindowName;
		Vec2f mPosition;
		Vec2f mWindowSize;

		bool mIsOpen;
		bool mIsDirty;
		bool mIsFocused;
		bool mIsHovered;
	};
}
