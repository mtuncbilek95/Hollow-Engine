#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Object.h>
#include <Runtime/Resource/Core/ResourceDescriptor.h>

namespace Hollow
{
	class RUNTIME_API ResourceSubObject;

	class RUNTIME_API ResourceObject : public Object
	{
	public:
		static SharedPtr<ResourceObject> CreateResource(const ResourceDescriptor& desc);

	public:
		ResourceObject(const ResourceDescriptor& desc);
		virtual ~ResourceObject() override = default;

	private:
		SharedPtr<ResourceSubObject> mOwnedSubObject;
	};
}
