    #pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Core/ResourceSubObject.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureBuffer.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>

namespace Hollow
{
	class RUNTIME_API TextureResource final : public ResourceSubObject
	{
	public:
		TextureResource();
		virtual ~TextureResource() override = default;

		SharedPtr<Texture> GetTexture() const { return mTexture; }
		SharedPtr<TextureBuffer> GetTextureBuffer() const { return mTextureBuffer; }

		void ConnectMemory(const SharedPtr<GraphicsMemory>& hostMemory, const SharedPtr<GraphicsMemory>& deviceMemory, bool bPreAllocate = true);
		void CreateTextureAndBuffer(const TextureDesc& desc);

		void UpdateTextureAndBuffer(MemoryBuffer& pBuffer, u32 offset);

		virtual void OnShutdown() noexcept override;
		virtual ResourceObjectType GetObjectType() const noexcept final { return ResourceObjectType::Texture; }

	private:
		void CreateInternalResources();

	private:
		SharedPtr<GraphicsDevice> mGraphicsDevice;
		SharedPtr<Texture> mTexture;
		SharedPtr<TextureBuffer> mTextureBuffer;
		SharedPtr<GraphicsBuffer> mStageBuffer;

		SharedPtr<CommandBuffer> mCommandBuffer;
		SharedPtr<CommandPool> mCommandPool;
		SharedPtr<Fence> mFence;

		SharedPtr<GraphicsMemory> mHostMemory;
		SharedPtr<GraphicsMemory> mDeviceMemory;

		bool mPreAllocate;
		TextureDesc mDesc;
	};
}
