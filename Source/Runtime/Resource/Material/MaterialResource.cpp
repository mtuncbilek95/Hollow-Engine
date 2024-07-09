#include "MaterialResource.h"

#include <Runtime/Graphics/API/GraphicsManager.h>
#include <Runtime/Graphics/Texture/TextureBuffer.h>
#include <Runtime/Graphics/Sampler/Sampler.h>

namespace Hollow
{
	//MaterialResource::MaterialResource()
	//{
	//	mGraphicsDevice = GraphicsManager::GetAPI().GetDefDevice();
	//	CreateInternalResources();
	//}

	//void MaterialResource::CreateInternalResources()
	//{
	//	CommandPoolDesc poolDesc = { CommandPoolType::Graphics };
	//	mCommandPool = mGraphicsDevice->CreateCommandPool(poolDesc);

	//	CommandBufferDesc bufferDesc = { mCommandPool };
	//	mCommandBuffer = mGraphicsDevice->CreateCommandBuffer(bufferDesc);

	//	FenceDesc fenceDesc = { false };
	//	mFence = mGraphicsDevice->CreateSyncFence(fenceDesc);
	//}
}
