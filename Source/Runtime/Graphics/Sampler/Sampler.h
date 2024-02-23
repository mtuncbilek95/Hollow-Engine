#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Sampler/SamplerDesc.h>

namespace Hollow
{
	class RUNTIME_API Sampler : public GraphicsDeviceObject
	{
	public:
		Sampler(const SamplerDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), 
			mMinFilter(desc.MinFilter), mMagFilter(desc.MagFilter), mMipmapMode(desc.MipmapMode),
			mAddressModeU(desc.AddressModeU), mAddressModeV(desc.AddressModeV), mAddressModeW(desc.AddressModeW),
			mMipLodBias(desc.MipLodBias), mMaxAnisotropy(desc.MaxAnisotropy), mComparisonEnabled(desc.bComparisonEnabled),
			mCompareOperation(desc.CompareOperation), mMinLod(desc.MinLod), mMaxLod(desc.MaxLod), mBorderColor(desc.BorderColor)
		{
		}

		virtual ~Sampler() override = default;

		SamplerFiltering GetMinFilter() const noexcept { return mMinFilter; }
		SamplerFiltering GetMagFilter() const noexcept { return mMagFilter; }
		SamplerMapMode GetMipmapMode() const noexcept { return mMipmapMode; }
		SamplerAddressMode GetAddressModeU() const noexcept { return mAddressModeU; }
		SamplerAddressMode GetAddressModeV() const noexcept { return mAddressModeV; }
		SamplerAddressMode GetAddressModeW() const noexcept { return mAddressModeW; }
		float GetMipLodBias() const noexcept { return mMipLodBias; }
		float GetMaxAnisotropy() const noexcept { return mMaxAnisotropy; }
		bool IsComparisonEnabled() const noexcept { return mComparisonEnabled; }
		CompareOperation GetCompareOperation() const noexcept { return mCompareOperation; }
		float GetMinLod() const noexcept { return mMinLod; }
		float GetMaxLod() const noexcept { return mMaxLod; }
		SamplerBorderColor GetBorderColor() const noexcept { return mBorderColor; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::Sampler; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		SamplerFiltering mMinFilter;
		SamplerFiltering mMagFilter;
		SamplerMapMode mMipmapMode;
		SamplerAddressMode mAddressModeU;
		SamplerAddressMode mAddressModeV;
		SamplerAddressMode mAddressModeW;
		float mMipLodBias;
		float mMaxAnisotropy;
		bool mComparisonEnabled;
		CompareOperation mCompareOperation;
		float mMinLod;
		float mMaxLod;
		SamplerBorderColor mBorderColor;
	};
}
