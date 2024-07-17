#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Memory/MemoryBuffer.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Command/CmdBufferDesc.h>
#include <Engine/Graphics/Command/Core/DynamicPassDesc.h>
#include <Engine/Graphics/Command/Core/GraphicsIndexType.h>
#include <Engine/Graphics/Pipeline/Core/ViewportDesc.h>
#include <Engine/Graphics/Pipeline/Core/ScissorDesc.h>
#include <Engine/Graphics/Shader/ShaderStage.h>

#include <Engine/Graphics/Command/Core/BufferBufferCopyDesc.h>
#include <Engine/Graphics/Command/Core/BufferTextureCopyDesc.h>
#include <Engine/Graphics/Command/Core/TextureImageBarrier.h>
#include <Engine/Graphics/Command/Core/BufferMemoryBarrier.h>

namespace Hollow
{
	class ENGINE_API GraphicsBuffer;
	class ENGINE_API Pipeline;
	class ENGINE_API DescriptorSet;
	class ENGINE_API TextureImage;
	class ENGINE_API CmdPool;

	class ENGINE_API CmdBuffer : public DeviceObject
	{
	public:
		CmdBuffer(const CmdBufferDesc& desc, WeakPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice) , mOwnerPool(desc.pOwnerPool), mRecording(false) 
		{}
		virtual ~CmdBuffer() override = default;

		void BeginRecording();
		void EndRecording();
		void BeginRendering(const DynamicPassDesc& desc);
		void EndRendering();
		void BindPipeline(WeakPtr<Pipeline> pPipeline);
		void BindVertexBuffers(WeakPtr<GraphicsBuffer> pBuffer[], u32 amount);
		void BindIndexBuffer(WeakPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType);
		void BindDescriptors(WeakPtr<DescriptorSet> ppSet[], u32 amount, u32 setIndex);
		void PushConstants(MemoryBuffer buffer, u32 offset, ShaderStage stage);
		void DrawIndexed(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount);
		void SetViewports(ViewportDesc pViewports[], u8 count);
		void SetScissors(ScissorDesc pScissors[], u8 count);

		void CopyBufferToBuffer(WeakPtr<GraphicsBuffer> pSourceBuffer, WeakPtr<GraphicsBuffer> pDestinationBuffer, BufferBufferCopyDesc& desc);
		void CopyBufferToTexture(WeakPtr<GraphicsBuffer> pSourceBuffer, WeakPtr<TextureImage> pDestinationTexture, BufferTextureCopyDesc& desc);
		void SetTextureBarrier(WeakPtr<TextureImage> pTexture, TextureImageBarrier& desc);
		void SetBufferBarrier(WeakPtr<GraphicsBuffer> pBuffer, BufferMemoryBarrier& desc);

		WeakPtr<CmdPool> GetOwnerPool() const noexcept { return mOwnerPool; }
		WeakPtr<GraphicsBuffer> GetBoundIndexBuffer() const noexcept { return mBoundIndexBuffer; }
		WeakPtr<Pipeline> GetBoundPipeline() const noexcept { return mBoundPipeline; }
		bool IsRecording() const noexcept { return mRecording; }

	protected:
		virtual void BeginRecordingImpl() = 0;
		virtual void EndRecordingImpl() = 0;
		virtual void BeginRenderingImpl(const DynamicPassDesc& desc) = 0;
		virtual void EndRenderingImpl() = 0;
		virtual void BindPipelineImpl(WeakPtr<Pipeline> pPipeline) = 0;
		virtual void BindVertexBuffersImpl(WeakPtr<GraphicsBuffer> ppBuffer[], u32 amount) = 0;
		virtual void BindIndexBufferImpl(WeakPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType) = 0;
		virtual void BindDescriptorsImpl(WeakPtr<DescriptorSet> ppSet[], u32 amount, u32 setIndex) = 0;
		virtual void PushConstantsImpl(MemoryBuffer buffer, u32 offset, ShaderStage stage) = 0;
		virtual void DrawIndexedImpl(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount) = 0;
		virtual void SetViewportsImpl(ViewportDesc pViewports[], u8 count) = 0;
		virtual void SetScissorsImpl(ScissorDesc pScissors[], u8 count) = 0;

		virtual void CopyBufferToBufferImpl(WeakPtr<GraphicsBuffer> pSourceBuffer, WeakPtr<GraphicsBuffer> pDestinationBuffer, BufferBufferCopyDesc& desc) = 0;
		virtual void CopyBufferToTextureImpl(WeakPtr<GraphicsBuffer> pSourceBuffer, WeakPtr<TextureImage> pDestinationTexture, BufferTextureCopyDesc& desc) = 0;
		virtual void SetTextureBarrierImpl(WeakPtr<TextureImage> pTexture, TextureImageBarrier& desc) = 0;
		virtual void SetBufferBarrierImpl(WeakPtr<GraphicsBuffer> pBuffer, BufferMemoryBarrier& desc) = 0;

	private:
		WeakPtr<CmdPool> mOwnerPool;
		WeakPtr<GraphicsBuffer> mBoundIndexBuffer;
		WeakPtr<Pipeline> mBoundPipeline;
		bool mRecording;
	};
}