#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Core/ResourceSubObject.h>

#include <Runtime/Resource/Material/MaterialResourceLayout.h>

#include <Runtime/Resource/Texture/TextureResource.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayout.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>

namespace Hollow
{
	class RUNTIME_API MaterialResource final : public ResourceSubObject
	{
		struct RUNTIME_API MaterialData
		{
			SharedPtr<TextureResource> BaseTexture;
			SharedPtr<TextureResource> NormalTexture;
			SharedPtr<TextureResource> MetallicTexture;
			SharedPtr<TextureResource> RoughnessTexture;
			SharedPtr<TextureResource> AOTexture;

			i32 MaterialIndex;
		};

	public:
		MaterialResource();
		~MaterialResource() override = default;

		void ConnectMemory(const SharedPtr<GraphicsMemory> deviceMemory, const SharedPtr<GraphicsMemory> hostMemory);
		void CreateDescriptorSet(const SharedPtr<DescriptorLayout> layout, const SharedPtr<DescriptorPool> pool);
		void ConnectMaterial(const SubMeshMaterial material);

	private:
		SharedPtr<GraphicsDevice> mDevice;
		SharedPtr<DescriptorSet> mMaterialSet;
		MaterialData mData;

		SharedPtr<GraphicsMemory> mDeviceMemory;
		SharedPtr<GraphicsMemory> mHostMemory;
	};
}
