#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanCommandBuffer : public CommandBuffer
	{
	public:
		VulkanCommandBuffer(const CommandBufferDesc& desc, VulkanDevice* pDevice);
		~VulkanCommandBuffer() override = default;

		VkCommandBuffer GetVkCommandBuffer() const noexcept { return mVkCommandBuffer; }
		VkDevice GetVkDevice() const noexcept { return mVkDevice; }

		// Inherited via CommandBuffer
		void OnShutdown() noexcept override;

	protected:
		virtual void BeginRecordingImpl() override;
		virtual void EndRecordingImpl() override;
		virtual void BeginRenderPassImpl(RenderPass** pRenderPasses, Vector4f colorPass) override;
		virtual void EndRenderPassImpl() override;
		virtual void BindPipelineImpl(Pipeline** pPipelines) override;
		virtual void BindVertexBuffersImpl(GraphicsBuffer** ppBuffer, uint32 amount) override;
		virtual void BindIndexBufferImpl(GraphicsBuffer* pBuffer, GraphicsIndexType indexType) override;
		virtual void BindDescriptorsImpl(DescriptorSet** ppSets, uint32 amount) override;
		virtual void DrawIndexedImpl(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset, uint32 instanceCount) override;
		virtual void SetViewportsImpl(ViewportDesc* pViewports, byte count) override;
		virtual void SetScissorsImpl(ScissorDesc* pScissors, byte count) override;
		virtual void CopyBufferToBufferImpl(GraphicsBuffer* pSourceBuffer, GraphicsBuffer* pDestinationBuffer, BufferBufferCopyDesc& desc) override;
		virtual void CopyBufferToTextureImpl(GraphicsBuffer* pSourceBuffer, Texture* pDestinationTexture, BufferTextureCopyDesc& desc) override;
		virtual void SetTextureBarrierImpl(Texture* pTexture, TextureBarrierUpdateDesc& desc) override;

	private:
		VkCommandBuffer mVkCommandBuffer;
		VkDevice mVkDevice;
	};
}