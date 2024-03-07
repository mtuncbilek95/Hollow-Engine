#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/RenderPass/RenderPassDesc.h>

namespace Hollow
{
	class RUNTIME_API RenderPass : public GraphicsDeviceObject
	{
	public:
		RenderPass(const RenderPassDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice) , mTargetRenderSize(desc.TargetRenderSize),
			mFormat(desc.Format), mSampleCount(desc.SampleCount), mColorLoadOperation(desc.ColorLoadOperation), mColorStoreOperation(desc.ColorStoreOperation),
			mStencilLoadOperation(desc.StencilLoadOperation), mStencilStoreOperation(desc.StencilStoreOperation), mInputLayout(desc.InputLayout), mOutputLayout(desc.OutputLayout),
			mArrayLevel(desc.ArrayLevel), mMipLevel(desc.MipLevel), mViews(desc.Views)
		{}
		virtual ~RenderPass() override = default;

		const Vector2u& GetTargetRenderSize() const noexcept { return mTargetRenderSize; }
		TextureFormat GetFormat() const noexcept { return mFormat; }
		TextureSampleCount GetSampleCount() const noexcept { return mSampleCount; }
		RenderPassLoadOperation GetColorLoadOperation() const noexcept { return mColorLoadOperation; }
		RenderPassStoreOperation GetColorStoreOperation() const noexcept { return mColorStoreOperation; }
		RenderPassLoadOperation GetStencilLoadOperation() const noexcept { return mStencilLoadOperation; }
		RenderPassStoreOperation GetStencilStoreOperation() const noexcept { return mStencilStoreOperation; }
		const TextureMemoryLayout& GetInputLayout() const noexcept { return mInputLayout; }
		const TextureMemoryLayout& GetOutputLayout() const noexcept { return mOutputLayout; }
		byte GetArrayLevel() const noexcept { return mArrayLevel; }
		byte GetMipLevel() const noexcept { return mMipLevel; }
		const Array<TextureView*>& GetViews() const noexcept { return mViews; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::RenderPass; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		Vector2u mTargetRenderSize;
		TextureFormat mFormat;
		TextureSampleCount mSampleCount;
		RenderPassLoadOperation mColorLoadOperation;
		RenderPassStoreOperation mColorStoreOperation;
		RenderPassLoadOperation mStencilLoadOperation;
		RenderPassStoreOperation mStencilStoreOperation;
		TextureMemoryLayout mInputLayout;
		TextureMemoryLayout mOutputLayout;
		byte mArrayLevel;
		byte mMipLevel;
		Array<TextureView*> mViews;
	};
}