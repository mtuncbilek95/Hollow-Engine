#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/ManagerAPI.h>
#include <Runtime/Resource/Mesh/MeshResource.h>

namespace Hollow
{
	class RUNTIME_API MeshLayoutFactory : public ManagerAPI<MeshLayoutFactory>
	{
	public:
		MeshLayoutFactory() = default;
		~MeshLayoutFactory() override = default;

		void AddMeshLayout(SharedPtr<MeshResource> layout)
		{
			const Guid& guid = layout->GetResourceID();
			mMeshLayouts[guid] = layout; 
		}

		SharedPtr<MeshResource> GetMeshLayout(const Guid& guid) { return mMeshLayouts[guid]; }

	private:
		HashMap<Guid, SharedPtr<MeshResource>> mMeshLayouts;
	};
}
