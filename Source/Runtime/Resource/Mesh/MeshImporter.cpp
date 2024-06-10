#include "MeshImporter.h"

#include <Runtime/Platform/PlatformFile.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Hollow
{
	MeshResourceLayout MeshImporter::Import(String path)
	{
		if (!PlatformFile::Exists(path))
		{
			CORE_LOG(HE_ERROR, "MeshImporter", "Failed to load mesh file with error: File does not exist");
			return MeshResourceLayout();
		}

		Assimp::Importer assimpImporter;

		const aiScene* scene = assimpImporter.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CORE_LOG(HE_ERROR, "MeshImporter", "Failed to load mesh file with error: %s", assimpImporter.GetErrorString());
			return MeshResourceLayout();
		}

		MeshResourceLayout mainLayout;

		for (i32 meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
		{
			// Define layout per mesh in the whole scene.
			SubMeshLayout subMeshLayout;

			// Get the mesh from the scene via Assimp.
			const aiMesh* mesh = scene->mMeshes[meshIndex];

			if(!mesh->HasPositions() || !mesh->HasNormals() || !mesh->HasTextureCoords(0) || !mesh->HasTangentsAndBitangents())
			{
				CORE_LOG(HE_ERROR, "MeshImporter", "Failed to load mesh file with error: Mesh does not have required data");
				return MeshResourceLayout();
			}

			// Define layout per vertex in the mesh.
			for (i32 vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++)
			{
				VertexData vertexData;

				// Get the vertex data from the mesh via Assimp.
				vertexData.Position = Vector3f(mesh->mVertices[vertexIndex].x, mesh->mVertices[vertexIndex].y, mesh->mVertices[vertexIndex].z);
				vertexData.Normal = Vector3f(mesh->mNormals[vertexIndex].x, mesh->mNormals[vertexIndex].y, mesh->mNormals[vertexIndex].z);
				vertexData.Tangent = Vector3f(mesh->mTangents[vertexIndex].x, mesh->mTangents[vertexIndex].y, mesh->mTangents[vertexIndex].z);
				vertexData.Bitangent = Vector3f(mesh->mBitangents[vertexIndex].x, mesh->mBitangents[vertexIndex].y, mesh->mBitangents[vertexIndex].z);
				vertexData.UV = Vector2f(mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y);
				vertexData.Color = Vector4f(0.5f, 0.5f, 0.5f, 1.0f);

				// Add the vertex data to the layout.
				subMeshLayout.Vertices.push_back(vertexData);
			}

			// Define layout per face in the mesh.
			for (i32 faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
			{
				// Get the face data from the mesh via Assimp.
				const aiFace& face = mesh->mFaces[faceIndex];

				for (i32 indexIndex = 0; indexIndex < face.mNumIndices; indexIndex++)
					subMeshLayout.Indices.push_back(face.mIndices[indexIndex]);
			}

			// Add the sub-mesh layout to the main layout.
			mainLayout.SubMeshes.push_back(subMeshLayout);
		}

		return mainLayout;
	}
}
