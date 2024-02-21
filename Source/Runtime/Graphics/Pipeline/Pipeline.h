#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Pipeline/GraphicsPipelineDesc.h>
#include <Runtime/Graphics/Pipeline/ComputePipelineDesc.h>

#include <Runtime/Graphics/Pipeline/PipelineBindPoint.h>

namespace Hollow
{
	class RUNTIME_API Pipeline : public GraphicsDeviceObject
	{
	public:
		Pipeline(const GraphicsPipelineDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),
			mBindPoint(PipelineBindPoint::Graphics), 
			mGraphicsShaders(desc.Shaders), mInputLayout(desc.InputLayout),
			mRasterizer(desc.Rasterizer), mDepthStencil(desc.DepthStencil), mBlend(desc.Blend), 
			mPrimitiveMode(desc.PrimitiveMode), mMultisample(desc.Multisample), mDescriptorLayout(desc.DescriptorLayout),
			mRenderPass(desc.pRenderPass), mViewport(desc.Viewport), mScissor(desc.Scissor), mSubpass(desc.Subpass)
		{}

		Pipeline(const ComputePipelineDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),
			mBindPoint(PipelineBindPoint::Compute),
			mComputeShader(desc.ComputeShader), mResourceLayouts(desc.DescriptorLayout)
		{}
		virtual ~Pipeline() override = default;

		const Array<SharedPtr<Shader>>& GetShaders() const { return mGraphicsShaders; }
		const SharedPtr<Shader>& GetComputeShader() const { return mComputeShader; }
		const InputLayoutDesc& GetInputLayout() const { return mInputLayout; }
		const MultisampleDesc& GetMultisample() const { return mMultisample; }
		const PipelineLayoutDesc& GetResourceLayout() const { return mDescriptorLayout; }
		const SharedPtr<RenderPass>& GetRenderPass() const { return mRenderPass; }
		const RasterizerDesc& GetRasterizer() const { return mRasterizer; }
		const DepthStencilDesc& GetDepthStencil() const { return mDepthStencil; }
		const BlendStateDesc& GetBlend() const { return mBlend; }
		const ViewportDesc& GetViewport() const { return mViewport; }
		const ScissorDesc& GetScissor() const { return mScissor; }
		PrimitiveMode GetPrimitiveMode() const { return mPrimitiveMode; }
		uint32 GetSubpass() const { return mSubpass; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::Pipeline; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		PipelineBindPoint mBindPoint;
		Array<SharedPtr<Shader>> mGraphicsShaders;
		InputLayoutDesc mInputLayout;
		RasterizerDesc mRasterizer;
		DepthStencilDesc mDepthStencil;
		BlendStateDesc mBlend;
		PrimitiveMode mPrimitiveMode;
		MultisampleDesc mMultisample;
		PipelineLayoutDesc mDescriptorLayout;
		SharedPtr<RenderPass> mRenderPass;
		ViewportDesc mViewport;
		ScissorDesc mScissor;
		uint32 mSubpass;

		Array<SharedPtr<PipelineLayoutDesc>> mResourceLayouts;
		SharedPtr<Shader> mComputeShader;
	};
}
