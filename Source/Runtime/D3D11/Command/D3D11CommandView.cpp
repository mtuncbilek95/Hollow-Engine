#include "D3D11CommandView.h"

#include <Runtime/D3D11/Buffer/D3D11GraphicsView.h>
#include <Runtime/D3D11/Texture/D3D11Texture.h>
#include <Runtime/D3D11/Texture/D3D11TextureView.h>
#include <Runtime/D3D11/Pipeline/D3D11Pipeline.h>
#include <Runtime/D3D11/Shader/D3D11Shader.h>
#include <Runtime/D3D11/RenderPass/D3D11RenderPass.h>



namespace Hollow
{
	D3D11CommandView::D3D11CommandView(const CommandViewDesc& desc, D3D11Device* pDevice) : CommandView(desc)
	{
		pDevice->GetD3DDevice()->CreateDeferredContext(0, &mD3DDeferredContext);
	}

	void D3D11CommandView::BeginRecordingCore()
	{
		// Nothing to do here
	}

	void D3D11CommandView::EndRecordingCore()
	{
		mD3DDeferredContext->FinishCommandList(false, &mD3DCommandList);
	}

	void D3D11CommandView::BeginRenderPassCore(SharedPtr<RenderPass>& renderPass, const Vector4f& clearColor)
	{
		// Clear the render target
		float color[4] = { clearColor.x, clearColor.y, clearColor.z, clearColor.w };
		ID3D11RenderTargetView* pRenderTargetView = std::dynamic_pointer_cast<D3D11RenderPass>(renderPass)->GetRenderTargetViews()[0].Get();
		mD3DDeferredContext->ClearRenderTargetView(pRenderTargetView, color);

		// Clear the depth stencil
		ID3D11DepthStencilView* pDepthStencilView = std::dynamic_pointer_cast<D3D11RenderPass>(renderPass)->GetDepthStencilView().Get();
		mD3DDeferredContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void D3D11CommandView::EndRenderPassCore()
	{
		// Nothing to do here
	}

	void D3D11CommandView::BindVertexBufferCore(const SharedPtr<GraphicsView>& vertexBuffer)
	{
		ID3D11Buffer* pVertexBuffer = std::dynamic_pointer_cast<D3D11GraphicsView>(vertexBuffer)->GetD3DBuffer().Get();

		UINT stride = vertexBuffer->GetStructureByteStride();
		UINT offset = 0;

		mD3DDeferredContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	}

	void D3D11CommandView::BindIndexBufferCore(const SharedPtr<GraphicsView>& indexBuffer, const IndexType type)
	{
		ID3D11Buffer* pIndexBuffer = std::dynamic_pointer_cast<D3D11GraphicsView>(indexBuffer)->GetD3DBuffer().Get();

		DXGI_FORMAT format = type == IndexType::Index16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;

		mD3DDeferredContext->IASetIndexBuffer(pIndexBuffer, format, 0);
	}

	void D3D11CommandView::BindGraphicsPipelineCore(const SharedPtr<Pipeline>& pipeline)
	{
		// Bind the shaders
		for (auto& shader : pipeline->GetShaders())
		{
			switch (shader->GetType())
			{
				case ShaderType::Vertex:
				{
					mD3DDeferredContext->VSSetShader(std::dynamic_pointer_cast<D3D11Shader>(shader)->GetD3DVertexShader().Get(), nullptr, 0);
					break;
				}
				case ShaderType::Pixel:
				{
					mD3DDeferredContext->PSSetShader(std::dynamic_pointer_cast<D3D11Shader>(shader)->GetD3DPixelShader().Get(), nullptr, 0);
					break;
				}
				case ShaderType::Geometry:
				{
					mD3DDeferredContext->GSSetShader(std::dynamic_pointer_cast<D3D11Shader>(shader)->GetD3DGeometryShader().Get(), nullptr, 0);
					break;
				}
				case ShaderType::Hull:
				{
					mD3DDeferredContext->HSSetShader(std::dynamic_pointer_cast<D3D11Shader>(shader)->GetD3DHullShader().Get(), nullptr, 0);
					break;
				}
				case ShaderType::Domain:
				{
					mD3DDeferredContext->DSSetShader(std::dynamic_pointer_cast<D3D11Shader>(shader)->GetD3DDomainShader().Get(), nullptr, 0);
					break;
				}
				case ShaderType::Compute:
				{
					mD3DDeferredContext->CSSetShader(std::dynamic_pointer_cast<D3D11Shader>(shader)->GetD3DComputeShader().Get(), nullptr, 0);
					break;
				}
				default:
					CORE_LOG(HE_WARNING, "D3D11CommandView", "Invalid shader type %d", shader->GetType());
					break;
			}
		}

		// Bind the pipeline state
		mD3DDeferredContext->IASetInputLayout(std::dynamic_pointer_cast<D3D11Pipeline>(pipeline)->GetD3DInputLayout().Get());
		float factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		mD3DDeferredContext->OMSetBlendState(std::dynamic_pointer_cast<D3D11Pipeline>(pipeline)->GetD3DBlendState().Get(), factor, D3D11_APPEND_ALIGNED_ELEMENT);
		mD3DDeferredContext->OMSetDepthStencilState(std::dynamic_pointer_cast<D3D11Pipeline>(pipeline)->GetD3DDepthStencilState().Get(), 0);
		mD3DDeferredContext->RSSetState(std::dynamic_pointer_cast<D3D11Pipeline>(pipeline)->GetD3DRasterizerState().Get());
		mD3DDeferredContext->IASetPrimitiveTopology(std::dynamic_pointer_cast<D3D11Pipeline>(pipeline)->GetD3DPrimitiveTopology());
	}

	void D3D11CommandView::BindComputePipelineCore(const SharedPtr<Pipeline>& pipeline)
	{
		// You only have one job.
		mD3DDeferredContext->CSSetShader(std::dynamic_pointer_cast<D3D11Shader>(pipeline->GetComputeShader())->GetD3DComputeShader().Get(), nullptr, 0);
	}

	void D3D11CommandView::SetViewportCore(const ArrayList<ViewportDesc> viewports, const byte amount)
	{
		ArrayList<D3D11_VIEWPORT> d3dViewports(viewports.size());

		for (byte i = 0; i < amount; i++)
		{
			d3dViewports[i].TopLeftX = static_cast<float>(viewports.at(i).OffsetSize.x);
			d3dViewports[i].TopLeftY = static_cast<float>(viewports.at(i).OffsetSize.y);
			d3dViewports[i].Width = static_cast<float>(viewports.at(i).ImageSize.x);
			d3dViewports[i].Height = static_cast<float>(viewports.at(i).ImageSize.y);
			d3dViewports[i].MinDepth = static_cast<float>(viewports.at(i).DepthRange.x);
			d3dViewports[i].MaxDepth = static_cast<float>(viewports.at(i).DepthRange.y);
		}

		mD3DDeferredContext->RSSetViewports(viewports.size(), d3dViewports.data());
	}

	void D3D11CommandView::SetScissorCore(const ArrayList<ScissorDesc> scissors, const byte amount)
	{
		ArrayList<D3D11_RECT> d3dScissors(scissors.size());

		for (byte i = 0; i < amount; i++)
		{
			d3dScissors[i].left = static_cast<long>(scissors.at(i).OffsetSize.x);
			d3dScissors[i].top = static_cast<long>(scissors.at(i).OffsetSize.y);
			d3dScissors[i].right = static_cast<long>(scissors.at(i).OffsetSize.x + scissors.at(i).ImageSize.x);
			d3dScissors[i].bottom = static_cast<long>(scissors.at(i).OffsetSize.y + scissors.at(i).ImageSize.y);
		}

		mD3DDeferredContext->RSSetScissorRects(scissors.size(), d3dScissors.data());
	}

	void D3D11CommandView::DrawIndexedCore(const uint32 indexCount, const uint32 indexOffset, const uint32 vertexOffset)
	{
		// You only have one job.
		mD3DDeferredContext->DrawIndexed(indexCount, indexOffset, vertexOffset);
	}

	void D3D11CommandView::UpdateBufferCore(SharedPtr<GraphicsView> buffer, const GraphicsViewUpdateDesc& desc)
	{
		// Map the buffer
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		mD3DDeferredContext->Map(std::dynamic_pointer_cast<D3D11GraphicsView>(buffer)->GetD3DBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		// Copy the data
		memcpy(mappedResource.pData, desc.pData, desc.SizeInBytes);

		// Unmap the buffer
		mD3DDeferredContext->Unmap(std::dynamic_pointer_cast<D3D11GraphicsView>(buffer)->GetD3DBuffer().Get(), 0);
	}

	void D3D11CommandView::UpdateResourceLayoutCore(SharedPtr<ResourceLayout> layout, const ResourceLayoutDesc& desc)
	{
	}

	void D3D11CommandView::CopyBufferToBufferCore(const SharedPtr<GraphicsView>& srcBuffer, const SharedPtr<GraphicsView>& dstBuffer, const BufferBufferCopyDesc& desc)
	{
	}

	void D3D11CommandView::CopyBufferToTextureCore(const SharedPtr<GraphicsView>& srcBuffer, const SharedPtr<Texture>& dstTexture, const BufferTextureCopyDesc& desc)
	{
	}

	void D3D11CommandView::CopyTextureToTextureCore(const SharedPtr<Texture>& srcTexture, const SharedPtr<Texture>& dstTexture, const TextureTextureCopyDesc& desc)
	{
	}

	void D3D11CommandView::ClearTextureCore(const SharedPtr<Texture>& textureView, const Vector4f& clearColor)
	{
	}
}
