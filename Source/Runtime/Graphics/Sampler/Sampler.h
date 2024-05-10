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
			mMagFilter(desc.MagFilter), mMinFilter(desc.MinFilter), mUAddressMode(desc.UAddressMode), 
			mVAddressMode(desc.VAddressMode), mWAddressMode(desc.WAddressMode), mAnisotropyEnabled(desc.bAnisotropyEnabled), 
			mMaxAnisotropy(desc.MaxAnisotropy), mBorderColor(desc.BorderColor), mCompareEnabled(desc.bCompareEnabled), 
			mCompareOp(desc.CompareOp), mMapMode(desc.MapMode), mMipLODBias(desc.MipLODBias), mMinLOD(desc.MinLOD), 
			mMaxLOD(desc.MaxLOD) 
		{}
		~Sampler() override = default;

		SamplerFilter GetMagFilter() const { return mMagFilter; }
		SamplerFilter GetMinFilter() const { return mMinFilter; }
		SamplerAddressMode GetUAddressMode() const { return mUAddressMode; }
		SamplerAddressMode GetVAddressMode() const { return mVAddressMode; }
		SamplerAddressMode GetWAddressMode() const { return mWAddressMode; }
		bool IsAnisotropyEnabled() const { return mAnisotropyEnabled; }
		uint32 GetMaxAnisotropy() const { return mMaxAnisotropy; }
		SamplerBorderColor GetBorderColor() const { return mBorderColor; }
		bool IsCompareEnabled() const { return mCompareEnabled; }
		CompareOperation GetCompareOp() const { return mCompareOp; }
		SamplerMapMode GetMapMode() const { return mMapMode; }
		float GetMipLODBias() const { return mMipLODBias; }
		float GetMinLOD() const { return mMinLOD; }
		float GetMaxLOD() const { return mMaxLOD; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override { return GraphicsDeviceObjectType::Sampler; }
		virtual void OnShutdown() noexcept override = 0;

	protected:
		void SetMaxAnisotropy(uint32 newValue) { mMaxAnisotropy = newValue; }

	private:
		SamplerFilter mMagFilter;
		SamplerFilter mMinFilter;
		SamplerAddressMode mUAddressMode;
		SamplerAddressMode mVAddressMode;
		SamplerAddressMode mWAddressMode;
		bool mAnisotropyEnabled;
		uint32 mMaxAnisotropy;
		SamplerBorderColor mBorderColor;
		bool mCompareEnabled;
		CompareOperation mCompareOp;
		SamplerMapMode mMapMode;
		float mMipLODBias;
		float mMinLOD;
		float mMaxLOD;
	};
}