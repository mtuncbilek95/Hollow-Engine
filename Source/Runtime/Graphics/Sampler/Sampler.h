#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Sampler/SamplerDesc.h>

namespace Hollow
{
	class RUNTIME_API Sampler : public GraphicsDeviceObject
	{
	public:
		Sampler(const SamplerDesc& desc) : mFilter(desc.Filter), mAddressU(desc.AddressU),
			mAddressV(desc.AddressV), mAddressW(desc.AddressW), mMipLODBias(desc.MipLODBias),
			mMaxAnisotropy(desc.MaxAnisotropy), mSamplerComparison(desc.SamplerComparison),
			mMinLOD(desc.MinLOD), mMaxLOD(desc.MaxLOD)
		{
			mBorderColor[0] = desc.BorderColor[0];
			mBorderColor[1] = desc.BorderColor[1];
			mBorderColor[2] = desc.BorderColor[2];
			mBorderColor[3] = desc.BorderColor[3];
		}

		virtual ~Sampler() override = default;

		SamplerFilter GetFilter() const { return mFilter; }
		AddressMode GetAddressU() const { return mAddressU; }
		AddressMode GetAddressV() const { return mAddressV; }
		AddressMode GetAddressW() const { return mAddressW; }
		const float& GetMipLODBias() const { return mMipLODBias; }
		const uint32& GetMaxAnisotropy() const { return mMaxAnisotropy; }
		SamplerComparison GetSamplerComparison() const { return mSamplerComparison; }
		const float* GetBorderColor() const { return mBorderColor; }
		const float& GetMinLOD() const { return mMinLOD; }
		const float& GetMaxLOD() const { return mMaxLOD; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::Sampler; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		SamplerFilter mFilter;
		AddressMode mAddressU;
		AddressMode mAddressV;
		AddressMode mAddressW;
		float mMipLODBias;
		uint32 mMaxAnisotropy;
		SamplerComparison mSamplerComparison;
		float mBorderColor[4];
		float mMinLOD;
		float mMaxLOD;
	};
}
