#include "RenderPass.h"

namespace Hollow
{
	RenderPass::RenderPass(const RenderPassDesc& desc, SharedPtr<GraphicsDevice> device) : GraphicsDeviceObject(device), mColorAttachments(desc.ColorAttachments), 
		mDepthStencilAttachment(desc.DepthStencilAttachment), mHasDepthStencilAttachment(desc.bHasDepthStencilAttachment), mInputAttachments(desc.InputAttachments), 
		mResolveAttachment(desc.ResolveAttachment), mHasResolveAttachment(desc.bHasResolveAttachment)
	{
	}
}
