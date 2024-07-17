#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Command/CmdBuffer.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class ENGINE_API VDevice;
	class VCmdBuffer : public CmdBuffer
	{
	public:
		VCmdBuffer(const CmdBufferDesc& desc, WeakPtr<VDevice> pDevice);
		~VCmdBuffer() override = default;

		VkCommandBuffer GetVkCmdBuffer() const { return mVkCmdBuffer; }

	protected:
		virtual void BeginRecordingImpl() override;
		virtual void EndRecordingImpl() override;
		virtual void BeginRenderingImpl(const DynamicPassDesc& desc) override;
		virtual void EndRenderingImpl() override;
		virtual void BindPipelineImpl(WeakPtr<Pipeline> pPipeline) override;
		virtual void BindVertexBuffersImpl(WeakPtr<GraphicsBuffer> ppBuffer[], u32 amount) override;
		virtual void BindIndexBufferImpl(WeakPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType) override;
		virtual void BindDescriptorsImpl(WeakPtr<DescriptorSet> ppSet[], u32 amount, u32 setIndex) override;
		virtual void PushConstantsImpl(MemoryBuffer buffer, u32 offset, ShaderStage stage) override;
		virtual void DrawIndexedImpl(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount) override;
		virtual void SetViewportsImpl(ViewportDesc pViewports[], u8 count) override;
		virtual void SetScissorsImpl(ScissorDesc pScissors[], u8 count) override;

		virtual void CopyBufferToBufferImpl(WeakPtr<GraphicsBuffer> pSourceBuffer, WeakPtr<GraphicsBuffer> pDestinationBuffer, BufferBufferCopyDesc& desc) override;
		virtual void CopyBufferToTextureImpl(WeakPtr<GraphicsBuffer> pSourceBuffer, WeakPtr<TextureImage> pDestinationTexture, BufferTextureCopyDesc& desc) override;
		virtual void SetTextureBarrierImpl(WeakPtr<TextureImage> pTexture, TextureImageBarrier& desc) override;
		virtual void SetBufferBarrierImpl(WeakPtr<GraphicsBuffer> pBuffer, BufferMemoryBarrier& desc) override;

	private:
		VkCommandBuffer mVkCmdBuffer;
		VkCommandPool mVkCmdPool;
		VkDevice mDevice;
	};
}
