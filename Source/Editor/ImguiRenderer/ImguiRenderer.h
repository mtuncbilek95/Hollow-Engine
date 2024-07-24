#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h>

#include "imgui.h"

namespace Hollow
{
	class ENGINE_API Fence;
	class ENGINE_API GraphicsQueue;
	class ENGINE_API CmdPool;
	class ENGINE_API CmdBuffer;
	class ENGINE_API DescriptorPool;
	class EDITOR_API ImguiRenderer : public IObject
	{
	public:
		ImguiRenderer();
		~ImguiRenderer();

		void BeginFrame();
		void EndFrame();

	private:
		ImGuiContext* mContext;

		SharedPtr<DescriptorPool> mDescriptorPool;

		SharedPtr<CmdBuffer> mCmdBuffer;
		SharedPtr<CmdPool> mCmdPool;

		WeakPtr<GraphicsQueue> mGraphicsQueue;

		SharedPtr<Fence> mFence;
	};
}
