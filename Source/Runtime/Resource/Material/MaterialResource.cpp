#include "MaterialResource.h"

#include <Runtime/Graphics/API/GraphicsManager.h>

namespace Hollow
{
	MaterialResource::MaterialResource()
	{
		mDevice = GraphicsManager::GetAPI().GetDefaultDevice();
	}

	void MaterialResource::ConnectMemory(const SharedPtr<GraphicsMemory> deviceMemory, const SharedPtr<GraphicsMemory> hostMemory)
	{
		mDeviceMemory = deviceMemory;
		mHostMemory = hostMemory;
	}

	void MaterialResource::CreateDescriptorSet(const SharedPtr<DescriptorLayout> layout, const SharedPtr<DescriptorPool> pool)
	{
		DescriptorSetDesc desc = {};
		desc.pLayout = layout;
		desc.pOwnerPool = pool;
		mMaterialSet = mDevice->CreateDescriptorSet(desc);
	}

	void MaterialResource::ConnectMaterial(const SubMeshMaterial material)
	{
		TextureDesc texDesc = {};
		texDesc.ArraySize = 1;
		texDesc.MipLevels = 1;
		texDesc.ImageFormat = TextureFormat::RGBA8_UNorm;
		texDesc.ImageSize = { material.BaseTexture->ImageSize.x, material.BaseTexture->ImageSize.y, 1 };
		texDesc.SampleCount = TextureSampleCount::Sample1;
		texDesc.Type = TextureType::Texture2D;
		texDesc.Usage = TextureUsage::ColorAttachment;
		texDesc.pMemory = mDeviceMemory;

		mData.BaseTexture = MakeShared<TextureResource>();
		mData.BaseTexture->ConnectMemory(mDeviceMemory, mHostMemory);
		mData.BaseTexture->CreateTextureAndBuffer(texDesc);
	}
}
