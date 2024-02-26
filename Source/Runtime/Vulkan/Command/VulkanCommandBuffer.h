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

		VkCommandBuffer GetVkCommandBuffer() const { return mVkCommandBuffer; }
		VkCommandPool GetVkCommandPool() const { return mVkCommandPool; }

		void OnShutdown() noexcept override;

	protected:
		virtual void BeginRecordingCore() override;
		virtual void EndRecordingCore() override;
		virtual void BindVertexBuffersCore(GraphicsBuffer** vertexBuffer, byte amount) override;
		virtual void BindIndexBufferCore(const GraphicsBuffer* indexBuffer, const IndexSizeType type) override;
		virtual void DrawIndexedCore(const uint32 indexCount, const uint32 indexOffset, const uint32 vertexOffset) override;
		virtual void DispatchComputeCore(const uint32 groupCountX, const uint32 groupCountY, const uint32 groupCountZ) override;
		virtual void BindPipelineCore(const Pipeline* pipeline) override;
		virtual void BeginRenderPassCore(RenderPass* renderPass, const Vector4f& clearColor, const byte ColorValueCount, const double clearDepth, const double clearStencil) override;
		virtual void EndRenderPassCore() override;
		virtual void SetViewportsCore(ViewportDesc* viewports, const byte amount) override;
		virtual void SetScissorsCore(ScissorDesc* scissors, const byte amount) override;
		virtual void CopyBufferToBufferCore(const GraphicsBuffer* srcBuffer, const GraphicsBuffer* dstBuffer, const BufferBufferCopyDesc& desc) override;
		virtual void CopyBufferToTextureCore(const GraphicsBuffer* srcBuffer, const Texture* dstTexture, const BufferTextureCopyDesc& desc) override;
		virtual void CopyTextureToTextureCore(const Texture* srcTexture, const Texture* dstTexture, const TextureTextureCopyDesc& desc) override;
		virtual void SetTextureMemoryBarrierCore(const Texture* pTexture, const TextureMemoryBarrierDesc& desc) override;
		virtual void SetBufferMemoryBarrierCore(const GraphicsBuffer* pBuffer, const BufferMemoryBarrierDesc& desc) override;
		virtual void CommitResourceSetsCore(DescriptorSet** ppSets, const byte amount) override;

	private:
		VkCommandBuffer mVkCommandBuffer;
		VkCommandPool mVkCommandPool;
		VkDevice mVkLogicalDevice;
	};
}