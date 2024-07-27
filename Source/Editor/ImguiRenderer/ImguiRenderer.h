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

	class EDITOR_API ImGuiRenderer : public IObject
	{
	public:
		ImGuiRenderer();
		~ImGuiRenderer();

		WeakPtr<CmdBuffer> GetCmdBuffer() { return mCmdBuffer; }
		WeakPtr<CmdPool> GetCmdPool() { return mCmdPool; }
		WeakPtr<GraphicsQueue> GetGraphicsQueue() { return mGraphicsQueue; }

	private:
		void InitVk();

	private:
		ImGuiContext* mContext;

		SharedPtr<DescriptorPool> mDescriptorPool;

		WeakPtr<GraphicsQueue> mGraphicsQueue;
		SharedPtr<Fence> mFence;

		SharedPtr<CmdBuffer> mCmdBuffer;
		SharedPtr<CmdPool> mCmdPool;
	};
}
