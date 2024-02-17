#include "D3D11Pipeline.h"

#include <Runtime/D3D11/Pipeline/D3D11PipelineUtils.h>
#include <Runtime/D3D11/Texture/D3D11TextureUtils.h>
#include <Runtime/D3D11/Shader/D3D11Shader.h>

namespace Hollow
{
	D3D11Pipeline::D3D11Pipeline(const GraphicsPipelineDesc& desc, D3D11Device* pDevice) : Pipeline(desc)
	{
		ArrayList<D3D11_INPUT_ELEMENT_DESC> inputElementDescs;
		inputElementDescs.reserve(desc.InputLayout.SemanticName.size());

		int i = 0;
		for (auto& semanticName : desc.InputLayout.SemanticName)
		{
			switch (semanticName)
			{
			case SemanticName::Position:
			{
				inputElementDescs[i].SemanticName = "POSITION";
				inputElementDescs[i].AlignedByteOffset = desc.InputLayout.AlignedByteOffset[i];
				inputElementDescs[i].Format = D3D11TextureUtils::GetDXTextureFormat(desc.InputLayout.InputFormat[i]);
				inputElementDescs[i].InputSlot = desc.InputLayout.InputSlotIndex[i];
				inputElementDescs[i].InputSlotClass = D3D11PipelineUtils::GetInputClassification(desc.InputLayout.SlotClass[i]);
				inputElementDescs[i].InstanceDataStepRate = desc.InputLayout.InstanceDataStep[i];
				inputElementDescs[i].SemanticIndex = desc.InputLayout.SemanticIndex[i];
				break;
			}
			case SemanticName::Normal:
			{
				inputElementDescs[i].SemanticName = "NORMAL";
				inputElementDescs[i].AlignedByteOffset = desc.InputLayout.AlignedByteOffset[i];
				inputElementDescs[i].Format = D3D11TextureUtils::GetDXTextureFormat(desc.InputLayout.InputFormat[i]);
				inputElementDescs[i].InputSlot = desc.InputLayout.InputSlotIndex[i];
				inputElementDescs[i].InputSlotClass = D3D11PipelineUtils::GetInputClassification(desc.InputLayout.SlotClass[i]);
				inputElementDescs[i].InstanceDataStepRate = desc.InputLayout.InstanceDataStep[i];
				inputElementDescs[i].SemanticIndex = desc.InputLayout.SemanticIndex[i];
				break;
			}
			case SemanticName::TexCoord:
			{
				inputElementDescs[i].SemanticName = "TEXCOORD";
				inputElementDescs[i].AlignedByteOffset = desc.InputLayout.AlignedByteOffset[i];
				inputElementDescs[i].Format = D3D11TextureUtils::GetDXTextureFormat(desc.InputLayout.InputFormat[i]);
				inputElementDescs[i].InputSlot = desc.InputLayout.InputSlotIndex[i];
				inputElementDescs[i].InputSlotClass = D3D11PipelineUtils::GetInputClassification(desc.InputLayout.SlotClass[i]);
				inputElementDescs[i].InstanceDataStepRate = desc.InputLayout.InstanceDataStep[i];
				inputElementDescs[i].SemanticIndex = desc.InputLayout.SemanticIndex[i];
				break;
			}
			case SemanticName::Tangent:
			{
				inputElementDescs[i].SemanticName = "TANGENT";
				inputElementDescs[i].AlignedByteOffset = desc.InputLayout.AlignedByteOffset[i];
				inputElementDescs[i].Format = D3D11TextureUtils::GetDXTextureFormat(desc.InputLayout.InputFormat[i]);
				inputElementDescs[i].InputSlot = desc.InputLayout.InputSlotIndex[i];
				inputElementDescs[i].InputSlotClass = D3D11PipelineUtils::GetInputClassification(desc.InputLayout.SlotClass[i]);
				inputElementDescs[i].InstanceDataStepRate = desc.InputLayout.InstanceDataStep[i];
				inputElementDescs[i].SemanticIndex = desc.InputLayout.SemanticIndex[i];
				break;
			}
			case SemanticName::BiNormal:
			{
				inputElementDescs[i].SemanticName = "BINORMAL";
				inputElementDescs[i].AlignedByteOffset = desc.InputLayout.AlignedByteOffset[i];
				inputElementDescs[i].Format = D3D11TextureUtils::GetDXTextureFormat(desc.InputLayout.InputFormat[i]);
				inputElementDescs[i].InputSlot = desc.InputLayout.InputSlotIndex[i];
				inputElementDescs[i].InputSlotClass = D3D11PipelineUtils::GetInputClassification(desc.InputLayout.SlotClass[i]);
				inputElementDescs[i].InstanceDataStepRate = desc.InputLayout.InstanceDataStep[i];
				inputElementDescs[i].SemanticIndex = desc.InputLayout.SemanticIndex[i];
				break;
			}
			case SemanticName::Color:
			{
				inputElementDescs[i].SemanticName = "COLOR";
				inputElementDescs[i].AlignedByteOffset = desc.InputLayout.AlignedByteOffset[i];
				inputElementDescs[i].Format = D3D11TextureUtils::GetDXTextureFormat(desc.InputLayout.InputFormat[i]);
				inputElementDescs[i].InputSlot = desc.InputLayout.InputSlotIndex[i];
				inputElementDescs[i].InputSlotClass = D3D11PipelineUtils::GetInputClassification(desc.InputLayout.SlotClass[i]);
				inputElementDescs[i].InstanceDataStepRate = desc.InputLayout.InstanceDataStep[i];
				inputElementDescs[i].SemanticIndex = desc.InputLayout.SemanticIndex[i];
				break;
			}
			default:
				CORE_LOG(HE_WARNING, "D3D11Pipeline", "Invalid semantic name %d", semanticName);
				break;
			}

			i++;
		}

		// Create input layout
		for (auto& shader : desc.Shaders)
		{
			if (shader->GetType() == ShaderType::Vertex)
			{
				D3D11Shader* vertexShader = static_cast<D3D11Shader*>(shader.get());
				DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateInputLayout(inputElementDescs.data(), inputElementDescs.size(), vertexShader->GetD3DBlob()->GetBufferPointer(),
					vertexShader->GetD3DBlob()->GetBufferSize(), mD3DInputLayout.GetAddressOf())), "D3D11Pipeline", "Failed to create input layout");
			}
		}

		CORE_LOG(HE_VERBOSE, "D3D11GraphicsPipeline", "Input layout has been created successfully.");

		// Create rasterizer state
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D11PipelineUtils::GetFillMode(desc.Rasterizer.FillMode);
		rasterizerDesc.CullMode = D3D11PipelineUtils::GetCullMode(desc.Rasterizer.CullMode);
		rasterizerDesc.FrontCounterClockwise = D3D11PipelineUtils::GetFaceOrdering(desc.Rasterizer.FaceOrdering);
		rasterizerDesc.DepthBias = desc.Rasterizer.DepthBias;
		rasterizerDesc.DepthBiasClamp = desc.Rasterizer.DepthClamp;
		rasterizerDesc.SlopeScaledDepthBias = desc.Rasterizer.SlopeScaledDepthBias;
		rasterizerDesc.DepthClipEnable = desc.Rasterizer.DepthClipEnabled;
		rasterizerDesc.ScissorEnable = desc.Rasterizer.ScissorEnabled;
		rasterizerDesc.MultisampleEnable = desc.Rasterizer.MultisampleEnabled;
		rasterizerDesc.AntialiasedLineEnable = desc.Rasterizer.AntialiasedLineEnabled;

		DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateRasterizerState(&rasterizerDesc, mD3DRasterizerState.GetAddressOf())),
			"D3D11Pipeline", "Failed to create rasterizer state");

		CORE_LOG(HE_VERBOSE, "D3D11GraphicsPipeline", "Rasterizer state has been created successfully.");

		// Create depth stencil state
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = desc.DepthStencil.DepthEnable;
		depthStencilDesc.DepthWriteMask = D3D11PipelineUtils::GetDepthWriteMask(desc.DepthStencil.DepthMask);
		depthStencilDesc.DepthFunc = D3D11PipelineUtils::GetComparisonFunc(desc.DepthStencil.DepthComparisonFunc);
		depthStencilDesc.StencilEnable = desc.DepthStencil.StencilEnable;
		depthStencilDesc.StencilReadMask = desc.DepthStencil.StencilReadMask;
		depthStencilDesc.StencilWriteMask = desc.DepthStencil.StencilWriteMask;

		DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateDepthStencilState(&depthStencilDesc, mD3DDepthStencilState.GetAddressOf())),
						"D3D11Pipeline", "Failed to create depth stencil state");

		CORE_LOG(HE_VERBOSE, "D3D11GraphicsPipeline", "Depth stencil state has been created successfully.");

		// Create blend state
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = desc.Blend.AlphaToCoverageEnable;
		blendDesc.IndependentBlendEnable = desc.Blend.IndependentBlendEnable;

		D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
		rtBlendDesc.BlendEnable = desc.Blend.RenderTargetBlendDesc.BlendEnable;
		rtBlendDesc.SrcBlend = D3D11PipelineUtils::GetBlendFactor(desc.Blend.RenderTargetBlendDesc.SrcBlend);
		rtBlendDesc.DestBlend = D3D11PipelineUtils::GetBlendFactor(desc.Blend.RenderTargetBlendDesc.DestBlend);
		rtBlendDesc.BlendOp = D3D11PipelineUtils::GetBlendOp(desc.Blend.RenderTargetBlendDesc.BlendOp);
		rtBlendDesc.SrcBlendAlpha = D3D11PipelineUtils::GetBlendFactor(desc.Blend.RenderTargetBlendDesc.SrcBlendAlpha);
		rtBlendDesc.DestBlendAlpha = D3D11PipelineUtils::GetBlendFactor(desc.Blend.RenderTargetBlendDesc.DestBlendAlpha);

		blendDesc.RenderTarget[0] = rtBlendDesc;

		DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateBlendState(&blendDesc, mD3DBlendState.GetAddressOf())),
						"D3D11Pipeline", "Failed to create blend state");

		CORE_LOG(HE_VERBOSE, "D3D11GraphicsPipeline", "Blend state has been created successfully.");
	}

	D3D11Pipeline::D3D11Pipeline(const ComputePipelineDesc& desc, D3D11Device* pDevice) : Pipeline(desc)
	{
	}

	D3D11_PRIMITIVE_TOPOLOGY D3D11Pipeline::GetD3DPrimitiveTopology()
	{
		return D3D11PipelineUtils::GetPrimitiveTopology(GetPrimitiveMode());
	}
}
