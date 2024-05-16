#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/ManagerAPI.h>
#include <Runtime/Resource/Core/ResourceObject.h>

namespace Hollow
{
	class RUNTIME_API ResourceManager final : public ManagerAPI<ResourceManager>
	{
	public:
		ResourceManager() = default;
		~ResourceManager() override = default;

		SharedPtr<ResourceObject> CreateResource(const ResourceDescriptor& desc)
		{
			SharedPtr<ResourceObject> resource = ResourceObject::CreateResource(desc);
			mResources.push_back(resource);
			return resource;
		}

		const ArrayList<SharedPtr<ResourceObject>>& GetResources() const { return mResources; }

	private:
		ArrayList<SharedPtr<ResourceObject>> mResources;
	};
}
