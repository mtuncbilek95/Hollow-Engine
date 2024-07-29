#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h>

#include "imgui.h"

#include <vulkan/vulkan.h>

namespace Hollow
{
	class Fence;
	class GraphicsQueue;
	class CmdPool;
	class CmdBuffer;
	class DescriptorPool;

	class ImGuiRenderer : public IObject
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
