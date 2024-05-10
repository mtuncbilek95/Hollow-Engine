#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureSampleCount.h>
#include <Runtime/Graphics/RenderPass/RenderPassLoadOperation.h>
#include <Runtime/Graphics/RenderPass/RenderPassStoreOperation.h>
#include <Runtime/Graphics/Texture/TextureMemoryLayout.h>
#include <Runtime/Graphics/Texture/TextureView.h>

namespace Hollow
{
	struct RUNTIME_API RenderPassAttachmentDesc
	{
		TextureFormat Format;
		TextureSampleCount SampleCount;
		RenderPassLoadOperation LoadOperation;
		RenderPassStoreOperation StoreOperation;
		RenderPassLoadOperation StencilLoadOperation;
		RenderPassStoreOperation StencilStoreOperation;
		TextureMemoryLayout InputLayout;
		TextureMemoryLayout OutputLayout;
		byte arrayListLevel;
		byte MipLevel;
		TextureView* View;
	};
}