#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	class Material;
	class RUNTIME_API Mesh
	{
	public:
		Mesh();
		virtual ~Mesh() = default;

		void AddMaterial(const SharedPtr<Material>& material);
		bool RemoveMaterial(const SharedPtr<Material>& material);


	private:
		ArrayList<SharedPtr<Material>> mMaterials;
	};
}
