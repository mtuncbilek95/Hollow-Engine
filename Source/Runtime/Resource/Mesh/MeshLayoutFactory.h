#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/ManagerAPI.h>
#include <Runtime/Resource/Mesh/MeshResourceLayout.h>

namespace Hollow
{
	class RUNTIME_API MeshLayoutFactory : public ManagerAPI<MeshLayoutFactory>
	{
	public:
		MeshLayoutFactory() = default;
		~MeshLayoutFactory() override = default;

		void AddMeshLayout(const Guid& guid, const MeshResourceLayout& layout) { mMeshLayouts[guid] = layout; }
		const MeshResourceLayout& GetMeshLayout(const Guid& guid) { return mMeshLayouts[guid]; }

	private:
		HashMap<Guid, MeshResourceLayout> mMeshLayouts;
	};
}
