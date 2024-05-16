#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Object.h>
#include <Runtime/Resource/Core/ResourceObject.h>
#include <Runtime/Resource/Core/ResourceObjectType.h>

namespace Hollow
{
	class RUNTIME_API ResourceSubObject : public Object
	{
		friend class RUNTIME_API ResourceObject;
	public:
		ResourceSubObject() : Object() {}
		virtual ~ResourceSubObject() override = default;

		SharedPtr<ResourceObject> GetOwnerResource() const { return mOwnerResource; }

		virtual void OnShutdown() noexcept override = 0;
		virtual ResourceObjectType GetObjectType() const noexcept = 0;

	private:
		void SetOwnerResource(const SharedPtr<ResourceObject> owner) { mOwnerResource = owner; }

	private:
		SharedPtr<ResourceObject> mOwnerResource;
	};
}
