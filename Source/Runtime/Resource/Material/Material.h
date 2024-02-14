#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/TextureResource/ResourceType.h>


namespace Hollow
{
	class TextureResource;
	class Sampler;
	class ResourceLayout;

	class RUNTIME_API Material
	{
	public:
		Material();
		virtual ~Material() = default;

		void AddTexture(SharedPtr<TextureResource> texture);
		void AddSampler(SharedPtr<Sampler> sampler);
		bool BindResourceLayout();

		SharedPtr<ResourceLayout> GetResourceLayout() const;
		const ArrayList<SharedPtr<TextureResource>>& GetTextures(ResourceType type) const;
		const ArrayList<SharedPtr<Sampler>>& GetSamplers() const;

	private:
		ArrayList<SharedPtr<TextureResource>> mTextures;
		ArrayList<SharedPtr<Sampler>> mSamplers;
		SharedPtr<ResourceLayout> mResourceLayout;
	};
} 
