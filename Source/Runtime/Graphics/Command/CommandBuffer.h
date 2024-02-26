#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferUpdateDesc.h>
#include <Runtime/Graphics/Common/IndexSizeType.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Command/CommandBufferDesc.h>
#include <Runtime/Graphics/Command/ViewportDesc.h>
#include <Runtime/Graphics/Command/ScissorDesc.h>
#include <Runtime/Graphics/Command/BufferBufferCopyDesc.h>
#include <Runtime/Graphics/Command/BufferTextureCopyDesc.h>
#include <Runtime/Graphics/Command/TextureTextureCopyDesc.h>
#include <Runtime/Graphics/Command/TextureMemoryBarrierDesc.h>
#include <Runtime/Graphics/Command/BufferMemoryBarrierDesc.h>

namespace Hollow
{
	class CommandPool;
	class GraphicsBuffer;
	class Pipeline;
	class Texture;
	class RenderPass;
	class DescriptorSet;

	class RUNTIME_API CommandBuffer : public GraphicsDeviceObject
	{
	public:
		CommandBuffer(const CommandBufferDesc& desc, GraphicsDevice* pDevice);
		virtual ~CommandBuffer() override = default;

		const CommandPool* GetOwnerPool() const { return mOwnerPool; }
		const GraphicsBuffer* GetBoundIndexBuffer() const { return mBoundIndexBuffer; }
		const Pipeline* GetBoundPipeline() const { return mBoundPipeline; }
		bool IsRecording() const { return mIsRecording; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::CommandBuffer; }

		void BeginRecording();
		void EndRecording();
		void BindVertexBuffers(GraphicsBuffer** vertexBuffer, byte amount);
		void BindIndexBuffer(GraphicsBuffer* indexBuffer, const IndexSizeType type);
		void DrawIndexed(const uint32 indexCount, const uint32 indexOffset, const uint32 vertexOffset);
		void DispatchCompute(const uint32 groupCountX, const uint32 groupCountY, const uint32 groupCountZ);
		void BindPipeline(Pipeline* pipeline);
		void BeginRenderPass(RenderPass* renderPass, const Vector4f& clearColor, const byte ColorValueCount, const double clearDepth, const double clearStencil);
		void EndRenderPass();
		void SetViewports(ViewportDesc* viewports, const byte amount);
		void SetScissors(ScissorDesc* scissors, const byte amount);
		void CopyBufferToBuffer(const GraphicsBuffer* srcBuffer, const GraphicsBuffer* dstBuffer, const BufferBufferCopyDesc& desc);
		void CopyBufferToTexture(const GraphicsBuffer* srcBuffer, const Texture* dstTexture, const BufferTextureCopyDesc& desc);
		void CopyTextureToTexture(const Texture* srcTexture, const Texture* dstTexture, const TextureTextureCopyDesc& desc);
		void SetTextureMemoryBarrier(const Texture* pTexture, const TextureMemoryBarrierDesc& desc);
		void SetBufferMemoryBarrier(const GraphicsBuffer* pBuffer, const BufferMemoryBarrierDesc& desc);
		void CommitResourceSets(DescriptorSet** ppSets, const byte amount);

		virtual void OnShutdown() noexcept override = 0;

	protected:
		virtual void BeginRecordingCore() = 0;
		virtual void EndRecordingCore() = 0;
		virtual void BindVertexBuffersCore(GraphicsBuffer** vertexBuffer, byte amount) = 0;
		virtual void BindIndexBufferCore(GraphicsBuffer* indexBuffer, const IndexSizeType type) = 0;
		virtual void DrawIndexedCore(const uint32 indexCount, const uint32 indexOffset, const uint32 vertexOffset) = 0;
		virtual void DispatchComputeCore(const uint32 groupCountX, const uint32 groupCountY, const uint32 groupCountZ) = 0;
		virtual void BindPipelineCore(const Pipeline* pipeline) = 0;
		virtual void BeginRenderPassCore(RenderPass* renderPass, const Vector4f& clearColor, const byte ColorValueCount, const double clearDepth, const double clearStencil) = 0;
		virtual void EndRenderPassCore() = 0;
		virtual void SetViewportsCore(ViewportDesc* viewports, const byte amount) = 0;
		virtual void SetScissorsCore(ScissorDesc* scissors, const byte amount) = 0;
		virtual void CopyBufferToBufferCore(const GraphicsBuffer* srcBuffer, const GraphicsBuffer* dstBuffer, const BufferBufferCopyDesc& desc) = 0;
		virtual void CopyBufferToTextureCore(const GraphicsBuffer* srcBuffer, const Texture* dstTexture, const BufferTextureCopyDesc& desc) = 0;
		virtual void CopyTextureToTextureCore(const Texture* srcTexture, const Texture* dstTexture, const TextureTextureCopyDesc& desc) = 0;
		virtual void SetTextureMemoryBarrierCore(const Texture* pTexture, const TextureMemoryBarrierDesc& desc) = 0;
		virtual void SetBufferMemoryBarrierCore(const GraphicsBuffer* pBuffer, const BufferMemoryBarrierDesc& desc) = 0;
		virtual void CommitResourceSetsCore(DescriptorSet** ppSets, const byte amount) = 0;

		void ClearBounds();

	private:
		CommandPool* mOwnerPool;
		GraphicsBuffer* mBoundIndexBuffer;
		Pipeline* mBoundPipeline;
		RenderPass* mBoundRenderPass;
		bool mIsRecording;
	};
}
