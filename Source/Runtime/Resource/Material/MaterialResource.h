#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Core/ResourceSubObject.h>
#include <Runtime/Resource/Texture/TextureResource.h>

#include <Runtime/Graphics/Descriptor/DescriptorLayout.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Fence/Fence.h>

namespace Hollow
{
	class RUNTIME_API MaterialResource : public ResourceSubObject
	{
	//public:
	//	MaterialResource();
	//	~MaterialResource() = default;

	//	void AddTexture(SharedPtr<TextureResource> texture)
	//	{
	//		mTextures[texture->GetResourceID()].first = texture->GetSubType();
	//		mTextures[texture->GetResourceID()].second = texture->GetTextureBuffer();
	//	}

	//	virtual ResourceObjectType GetObjectType() const noexcept final { return ResourceObjectType::Material; }
	//private:
	//	void CreateInternalResources();

	//private:
	//	HashMap<Guid,Pair<TextureResourceSubType, SharedPtr<TextureBuffer>>> mTextures;
	//	SharedPtr<GraphicsBuffer> mTransformBuffer;

	//	SharedPtr<GraphicsDevice> mGraphicsDevice;
	//	SharedPtr<CommandBuffer> mCommandBuffer;
	//	SharedPtr<CommandPool> mCommandPool;
	//	SharedPtr<Fence> mFence;
	};
}
