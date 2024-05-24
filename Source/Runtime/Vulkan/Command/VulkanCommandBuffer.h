#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanCommandBuffer final : public CommandBuffer
	{
	public:
		VulkanCommandBuffer(const CommandBufferDesc& desc, const SharedPtr<VulkanDevice> pDevice);
		~VulkanCommandBuffer() override = default;

		VkCommandBuffer GetVkCommandBuffer() const { return mVkCommandBuffer; }
		VkDevice GetVkDevice() const { return mVkDevice; }

		void OnShutdown() noexcept override;

	protected:
		virtual void BeginRecordingImpl() override;
		virtual void EndRecordingImpl() override;
		virtual void BeginRenderingImpl(const DynamicPassDesc& desc) override;
		virtual void EndRenderingImpl() override;
		virtual void BeginRenderPassImpl(SharedPtr<RenderPass> pRenderPass, SharedPtr<Framebuffer> pFramebuffer, const Vector4f& clearColor, const Vector2f& depthStencil) override;
		virtual void EndRenderPassImpl() override;
		virtual void BindPipelineImpl(SharedPtr<Pipeline> pPipeline) override;
		virtual void BindVertexBuffersImpl(SharedPtr<GraphicsBuffer> ppBuffer[], uint32 amount) override;
		virtual void BindIndexBufferImpl(SharedPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType) override;
		virtual void BindDescriptorsImpl(SharedPtr<DescriptorSet> ppSet[], uint32 amount) override;
		virtual void DrawIndexedImpl(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset, uint32 instanceCount) override;
		virtual void SetViewportsImpl(ViewportDesc pViewports[], byte count) override;
		virtual void SetScissorsImpl(ScissorDesc pScissors[], byte count) override;
		virtual void CopyBufferToBufferImpl(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<GraphicsBuffer> pDestinationBuffer, BufferBufferCopyDesc& desc) override;
		virtual void CopyBufferToTextureImpl(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<Texture> pDestinationTexture, BufferTextureCopyDesc& desc) override;
		virtual void ResolveTextureImpl(SharedPtr<Texture> pSourceTexture, SharedPtr<Texture> pDestinationTexture) override;
		virtual void SetTextureBarrierImpl(SharedPtr<Texture> pTexture, TextureBarrierUpdateDesc& desc) override;
		virtual void SetBufferMemoryBarrierImpl(SharedPtr<GraphicsBuffer> pBuffer, BufferMemoryBarrierUpdateDesc& desc) override;
		virtual void SetCommonMemoryBarrierImpl(CommonMemoryBarrierUpdateDesc& desc) override;

	private:
		VkCommandBuffer mVkCommandBuffer;
		VkDevice mVkDevice;
	};
}
