#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Mesh/MeshResource.h>
#include <Runtime/Resource/Material/MaterialResource.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>

namespace Hollow
{
	struct RUNTIME_API RenderObject : public ResourceSubObject
	{
		RenderObject();
		~RenderObject() = default;

	private:
		SharedPtr<MeshResource> mMesh;
		SharedPtr<MaterialResource> mMaterial;

		SharedPtr<Pipeline> mPipeline;

		SharedPtr<DescriptorSet> mDescriptorSet;
		SharedPtr<DescriptorPool> mDescriptorPool;
	};
}
