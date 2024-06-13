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
		struct RUNTIME_API MipData
		{
			SharedPtr<TextureBuffer> MipTextureBuffer;
			SharedPtr<GraphicsBuffer> MipStageBuffer;
		};

	public:
		TextureResource();
		virtual ~TextureResource() override = default;

		SharedPtr<TextureBuffer> CreateResourceBuffer(const TextureAspectFlags flag, const byte mipLevel, const byte arrayLevel);
		SharedPtr<Texture> GetTexture() const noexcept { return mTexture; }

		void InitializeMipData(const SharedPtr<Texture> texture);

		void ConnectMemory(const SharedPtr<GraphicsMemory> hostMemory, const SharedPtr<GraphicsMemory> deviceMemory);
		void CreateTextureData(const TextureDesc& desc, bool preAllocMips = false, bool preAllocData = false);

		void UpdateTextureData(const MemoryBuffer& data, const Vector3u& offset, const TextureMemoryLayout inputMemoryLayout, const GraphicsMemoryAccessFlags inputAccessFlags, const PipelineStageFlags inputPipelineFlags, const GraphicsQueueType inputQueueType, const byte mipLevel, const byte arrayLevel);
		virtual void OnShutdown() noexcept override;
		virtual ResourceObjectType GetObjectType() const noexcept final { return ResourceObjectType::Texture; }

	private:
		void CreateInternalResources();

	private:
		SharedPtr<GraphicsDevice> mGraphicsDevice;
		ArrayList<ArrayList<MipData>> mMipData;
		SharedPtr<Texture> mTexture;

		SharedPtr<CommandBuffer> mCommandBuffer;
		SharedPtr<CommandPool> mCommandPool;
		SharedPtr<Fence> mFence;

		SharedPtr<GraphicsMemory> mHostMemory;
		SharedPtr<GraphicsMemory> mDeviceMemory;

		bool mMipDataInitialized;
		TextureDesc mDesc;
	};
}
