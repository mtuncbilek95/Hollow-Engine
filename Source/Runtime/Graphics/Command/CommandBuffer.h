#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Memory/MemoryBuffer.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Command/CommandBufferDesc.h>
#include <Runtime/Graphics/Core/DynamicPassDesc.h>
#include <Runtime/Graphics/Core/GraphicsIndexType.h>
#include <Runtime/Graphics/Core/ViewportDesc.h>
#include <Runtime/Graphics/Core/ScissorDesc.h>
#include <Runtime/Graphics/Core/BufferBufferCopyDesc.h>
#include <Runtime/Graphics/Core/BufferTextureCopyDesc.h>
#include <Runtime/Graphics/Core/TextureBarrierUpdateDesc.h>
#include <Runtime/Graphics/Core/BufferMemoryBarrierUpdateDesc.h>
#include <Runtime/Graphics/Core/CommonMemoryBarrierUpdateDesc.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>

namespace Hollow
{
	class RUNTIME_API GraphicsBuffer;
	class RUNTIME_API Pipeline;
	class RUNTIME_API DescriptorSet;
	class RUNTIME_API Texture;
	class RUNTIME_API RenderPass;
	class RUNTIME_API Framebuffer;

	/**
	 * @class CommandBuffer
	 * 
	 * @brief Represents a command buffer that can be recorded and submitted to the GPU.
	 */
	class RUNTIME_API CommandBuffer : public GraphicsDeviceObject
	{
	public:
		CommandBuffer(const CommandBufferDesc& desc, const SharedPtr<GraphicsDevice> pDevice) : GraphicsDeviceObject(pDevice) , mOwnerPool(desc.pOwnerPool), mBoundIndexBuffer(nullptr),
		mBoundPipeline(nullptr), mRecording(false) 
		{}
		virtual ~CommandBuffer() override = default;

		void BeginRecording();
		void EndRecording();
		void BeginRendering(const DynamicPassDesc& desc);
		void EndRendering();
		void BeginRenderPass(SharedPtr<RenderPass> pRenderPass, SharedPtr<Framebuffer> pFramebuffer, const Vec4f& clearColor, const Vec2f& depthStencil);
		void EndRenderPass();
		void BindPipeline(SharedPtr<Pipeline> pPipeline);
		void BindVertexBuffers(SharedPtr<GraphicsBuffer> pBuffer[], u32 amount);
		void BindIndexBuffer(SharedPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType);
		void BindDescriptors(SharedPtr<DescriptorSet> ppSet[], u32 amount, u32 setIndex);
		void DrawIndexed(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount);
		void SetViewports(ViewportDesc pViewports[], byte count);
		void SetScissors(ScissorDesc pScissors[], byte count);
		void CopyBufferToBuffer(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<GraphicsBuffer> pDestinationBuffer, BufferBufferCopyDesc& desc);
		void CopyBufferToTexture(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<Texture> pDestinationTexture, BufferTextureCopyDesc& desc);
		void ResolveTexture(SharedPtr<Texture> pSourceTexture, SharedPtr<Texture> pDestinationTexture);
		void SetTextureBarrier(SharedPtr<Texture> pTexture, TextureBarrierUpdateDesc& desc);
		void SetBufferMemoryBarrier(SharedPtr<GraphicsBuffer> pBuffer, BufferMemoryBarrierUpdateDesc& desc);
		void SetCommonMemoryBarrier(CommonMemoryBarrierUpdateDesc& desc);
		void PushConstants(MemoryBuffer buffer, u32 offset, ShaderStage stage);

		SharedPtr<CommandPool> GetOwnerPool() const noexcept { return mOwnerPool; }
		SharedPtr<GraphicsBuffer> GetBoundIndexBuffer() const noexcept { return mBoundIndexBuffer; }
		SharedPtr<Pipeline> GetBoundPipeline() const noexcept { return mBoundPipeline; }
		bool IsRecording() const noexcept { return mRecording; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept final { return GraphicsDeviceObjectType::CommandBuffer; }

	protected:
		virtual void BeginRecordingImpl() = 0;
		virtual void EndRecordingImpl() = 0;
		virtual void BeginRenderingImpl(const DynamicPassDesc& desc) = 0;
		virtual void EndRenderingImpl() = 0;
		virtual void BeginRenderPassImpl(SharedPtr<RenderPass> pRenderPass, SharedPtr<Framebuffer> pFramebuffer, const Vec4f& clearColor, const Vec2f& depthStencil) = 0;
		virtual void EndRenderPassImpl() = 0;
		virtual void BindPipelineImpl(SharedPtr<Pipeline> pPipeline) = 0;
		virtual void BindVertexBuffersImpl(SharedPtr<GraphicsBuffer> ppBuffer[], u32 amount) = 0;
		virtual void BindIndexBufferImpl(SharedPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType) = 0;
		virtual void BindDescriptorsImpl(SharedPtr<DescriptorSet> ppSet[], u32 amount, u32 setIndex) = 0;
		virtual void DrawIndexedImpl(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount) = 0;
		virtual void SetViewportsImpl(ViewportDesc pViewports[], byte count) = 0;
		virtual void SetScissorsImpl(ScissorDesc pScissors[], byte count) = 0;
		virtual void CopyBufferToBufferImpl(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<GraphicsBuffer> pDestinationBuffer, BufferBufferCopyDesc& desc) = 0;
		virtual void CopyBufferToTextureImpl(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<Texture> pDestinationTexture, BufferTextureCopyDesc& desc) = 0;
		virtual void ResolveTextureImpl(SharedPtr<Texture> pSourceTexture, SharedPtr<Texture> pDestinationTexture) = 0;
		virtual void SetTextureBarrierImpl(SharedPtr<Texture> pTexture, TextureBarrierUpdateDesc& desc) = 0;
		virtual void SetBufferMemoryBarrierImpl(SharedPtr<GraphicsBuffer> pBuffer, BufferMemoryBarrierUpdateDesc& desc) = 0;
		virtual void SetCommonMemoryBarrierImpl(CommonMemoryBarrierUpdateDesc& desc) = 0;
		virtual void PushConstantsImpl(MemoryBuffer buffer, u32 offset, ShaderStage stage) = 0;

	private:
		SharedPtr<CommandPool> mOwnerPool;
		SharedPtr<RenderPass> mBoundRenderPass;
		SharedPtr<Framebuffer> mBoundFramebuffer;
		SharedPtr<GraphicsBuffer> mBoundIndexBuffer;
		SharedPtr<Pipeline> mBoundPipeline;
		bool mRecording;
	};
}