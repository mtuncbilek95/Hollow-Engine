#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Mesh/MeshResource.h>
#include <Runtime/Resource/Material/MaterialResource.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>

namespace Hollow
{
	struct RUNTIME_API RenderObject
	{
		RenderObject();

	private:
		SharedPtr<MeshResource> Mesh;
		SharedPtr<MaterialResource> Material;
		SharedPtr<Pipeline> Pipeline;
		SharedPtr<DescriptorSet> TransformSet;
	};
}
