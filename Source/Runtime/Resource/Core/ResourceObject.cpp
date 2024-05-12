#include "ResourceObject.h"

namespace Hollow
{
	SharedPtr<ResourceObject> ResourceObject::CreateResource(const ResourceDescriptor& desc)
	{
		return SharedPtr<ResourceObject>();
	}

	ResourceObject::ResourceObject(const ResourceDescriptor& desc)
	{
	}
}
