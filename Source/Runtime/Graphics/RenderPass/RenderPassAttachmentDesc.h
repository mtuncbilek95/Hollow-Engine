#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPassLoadOperation.h>
#include <Runtime/Graphics/RenderPass/RenderPassStoreOperation.h>
#include <Runtime/Graphics/Texture/TextureMemoryLayout.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureSampleCount.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureView.h>

namespace Hollow
{
	struct RUNTIME_API RenderPassAttachmentDesc
	{
		TextureView* pView;
		TextureFormat Format;
		TextureSampleCount SampleCount;
		RenderPassLoadOperation ColorLoadOperation;
		RenderPassStoreOperation ColorStoreOperation;
		RenderPassLoadOperation StencilLoadOperation;
		RenderPassStoreOperation StencilStoreOperation;
		TextureMemoryLayout InputLayout;
		TextureMemoryLayout OutputLayout;
		byte ArrayLevel;
		byte MipLevel;
	};
}