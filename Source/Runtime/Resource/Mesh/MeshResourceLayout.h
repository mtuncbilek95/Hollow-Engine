#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Mesh/SubMeshObject.h>

namespace Hollow
{
	struct RUNTIME_API MeshResourceLayout
	{
		ArrayList<SubMeshObject> SubMeshes;
	};
}
