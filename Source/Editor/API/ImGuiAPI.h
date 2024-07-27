#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h>

#include <Editor/ImguiRenderer/ImGuiRenderer.h>

namespace Hollow
{
	class EDITOR_API ImGuiAPI : public API<ImGuiAPI>
	{
		friend class ImGuiRenderModule;

	public:
		ImGuiAPI() = default;
		virtual ~ImGuiAPI() override = default;

		WeakPtr<ImGuiRenderer> GetEditorRenderer() { return mEditorRenderer; }

	private:
		WeakPtr<ImGuiRenderer> CreateEditor()
		{
			if (!mEditorRenderer)
				mEditorRenderer = MakeShared<ImGuiRenderer>();

			return mEditorRenderer;
		}

	private:
		SharedPtr<ImGuiRenderer> mEditorRenderer;
	};
}
