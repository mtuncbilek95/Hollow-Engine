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
			SharedPtr<ResourceObject> resource = MakeShared<ResourceObject>(desc);
			mResources.push_back(resource);
			return resource;
		}

		const DArray<SharedPtr<ResourceObject>>& GetResources() const { return mResources; }

	private:
		DArray<SharedPtr<ResourceObject>> mResources;
	};
}
