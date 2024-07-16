#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Memory/MemoryBuffer.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Command/CommandBufferDesc.h>
#include <Engine/Graphics/Command/Core/DynamicPassDesc.h>
#include <Engine/Graphics/Command/Core/GraphicsIndexType.h>
#include <Engine/Graphics/Pipeline/Core/ViewportDesc.h>
#include <Engine/Graphics/Pipeline/Core/ScissorDesc.h>
#include <Engine/Graphics/Shader/ShaderStage.h>

namespace Hollow
{
	class ENGINE_API GraphicsBuffer;
	class ENGINE_API Pipeline;
	class ENGINE_API DescriptorSet;
	class ENGINE_API TextureImage;

	class ENGINE_API CommandBuffer : public DeviceObject
	{
	public:
		CommandBuffer(const CommandBufferDesc& desc, const SharedPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice) , mOwnerPool(desc.pOwnerPool), mBoundIndexBuffer(nullptr),
		mBoundPipeline(nullptr), mRecording(false) 
		{}
		virtual ~CommandBuffer() override = default;

		void BeginRecording();
		void EndRecording();
		void BeginRendering(const DynamicPassDesc& desc);
		void EndRendering();
		void BindPipeline(SharedPtr<Pipeline> pPipeline);
		void BindVertexBuffers(SharedPtr<GraphicsBuffer> pBuffer[], u32 amount);
		void BindIndexBuffer(SharedPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType);
		void BindDescriptors(SharedPtr<DescriptorSet> ppSet[], u32 amount, u32 setIndex);
		void PushConstants(MemoryBuffer buffer, u32 offset, ShaderStage stage);
		void DrawIndexed(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount);
		void SetViewports(ViewportDesc pViewports[], u8 count);
		void SetScissors(ScissorDesc pScissors[], u8 count);
		
		SharedPtr<CommandPool> GetOwnerPool() const noexcept { return mOwnerPool; }
		SharedPtr<GraphicsBuffer> GetBoundIndexBuffer() const noexcept { return mBoundIndexBuffer; }
		SharedPtr<Pipeline> GetBoundPipeline() const noexcept { return mBoundPipeline; }
		bool IsRecording() const noexcept { return mRecording; }

	protected:
		virtual void BeginRecordingImpl() = 0;
		virtual void EndRecordingImpl() = 0;
		virtual void BeginRenderingImpl(const DynamicPassDesc& desc) = 0;
		virtual void EndRenderingImpl() = 0;
		virtual void BindPipelineImpl(SharedPtr<Pipeline> pPipeline) = 0;
		virtual void BindVertexBuffersImpl(SharedPtr<GraphicsBuffer> ppBuffer[], u32 amount) = 0;
		virtual void BindIndexBufferImpl(SharedPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType) = 0;
		virtual void BindDescriptorsImpl(SharedPtr<DescriptorSet> ppSet[], u32 amount, u32 setIndex) = 0;
		virtual void PushConstantsImpl(MemoryBuffer buffer, u32 offset, ShaderStage stage) = 0;
		virtual void DrawIndexedImpl(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount) = 0;
		virtual void SetViewportsImpl(ViewportDesc pViewports[], u8 count) = 0;
		virtual void SetScissorsImpl(ScissorDesc pScissors[], u8 count) = 0;
		

	private:
		SharedPtr<CommandPool> mOwnerPool;
		SharedPtr<GraphicsBuffer> mBoundIndexBuffer;
		SharedPtr<Pipeline> mBoundPipeline;
		bool mRecording;
	};
}