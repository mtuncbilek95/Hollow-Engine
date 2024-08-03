#pragma once

#include <Engine/Core/Core.h>
#include <Editor/Object/IEditorObject.h>

namespace Hollow
{
	class GuiWindow : public IEditorObject
	{
	public:
		virtual ~GuiWindow() = default;

		virtual void OnInitialize() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnPaint() = 0;
		virtual void OnClose() = 0;

		const String& GetWindowName() const { return mWindowName; }
		const Vec2f& GetPosition() const { return mPosition; }
		const Vec2f& GetWindowSize() const { return mWindowSize; }

		bool IsOpen() const { return mIsOpen; }
		bool IsDirty() const { return mIsDirty; }

		void SetOpen(bool open) { mIsOpen = open; }
		void SetDirty(bool dirty) { mIsDirty = dirty; }

	protected:
		String mWindowName;
		Vec2f mPosition;
		Vec2f mWindowSize;

		bool mIsOpen;
		bool mIsDirty;
	};
}
