#include "ResourceImporter.h"

#if defined(STB_IMAGE_IMPLEMENTATION)
#include <stb_image.h>
#endif

#include <Runtime/Platform/PlatformAPI.h>
#include <Runtime/Platform/PlatformFile.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

namespace Hollow
{
	MeshResourceLayout ResourceImporter::ImportMesh(String path)
	{
		if (!PlatformFile::Exists(path))
		{
			CORE_LOG(HE_ERROR, "MeshImporter", "Failed to load mesh file with error: File does not exist");
			return MeshResourceLayout();
		}

		Assimp::Importer assimpImporter;

		const aiScene* scene = assimpImporter.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_SortByPType | aiProcess_OptimizeMeshes);

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

			if (!mesh->HasPositions() || !mesh->HasNormals() || !mesh->HasTextureCoords(0) || !mesh->HasTangentsAndBitangents())
			{
				CORE_LOG(HE_ERROR, "MeshImporter", "Failed to load mesh file with error: Mesh does not have required data");
				return MeshResourceLayout();
			}

			// Define layout per vertex in the mesh.
			for (i32 vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++)
			{
				VertexData vertexData;

				// Get the vertex data from the mesh via Assimp.
				vertexData.Position = Vec3f(mesh->mVertices[vertexIndex].x /70, mesh->mVertices[vertexIndex].y /70, mesh->mVertices[vertexIndex].z / 70);
				vertexData.Normal = Vec3f(mesh->mNormals[vertexIndex].x, mesh->mNormals[vertexIndex].y, mesh->mNormals[vertexIndex].z);
				vertexData.Tangent = Vec3f(mesh->mTangents[vertexIndex].x, mesh->mTangents[vertexIndex].y, mesh->mTangents[vertexIndex].z);
				vertexData.Bitangent = Vec3f(mesh->mBitangents[vertexIndex].x, mesh->mBitangents[vertexIndex].y, mesh->mBitangents[vertexIndex].z);
				vertexData.UV = Vec2f(mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y);
				vertexData.Color = Vec4f(0.5f, 0.5f, 0.5f, 1.0f);

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

			subMeshLayout.MaterialIndex = mesh->mMaterialIndex;

			// Add the sub-mesh layout to the main layout.
			mainLayout.SubMeshes.push_back(subMeshLayout);
		}
		assimpImporter.FreeScene();
		return mainLayout;
	}

	SkyboxResourceLayout ResourceImporter::ImportSkybox(String path)
	{
		if (!PlatformFile::Exists(path))
		{
			CORE_LOG(HE_ERROR, "MeshImporter", "Failed to load mesh file with error: File does not exist");
			return SkyboxResourceLayout();
		}

		Assimp::Importer assimpImporter;

		const aiScene* scene = assimpImporter.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace | aiProcess_GenNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CORE_LOG(HE_ERROR, "MeshImporter", "Failed to load mesh file with error: %s", assimpImporter.GetErrorString());
			return SkyboxResourceLayout();
		}

		SkyboxResourceLayout mainLayout;

		for (i32 meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
		{
			// Define layout per mesh in the whole scene.
			SkyboxLayout subMeshLayout;

			// Get the mesh from the scene via Assimp.
			const aiMesh* mesh = scene->mMeshes[meshIndex];

			if (!mesh->HasPositions() || !mesh->HasNormals() || !mesh->HasTextureCoords(0) || !mesh->HasTangentsAndBitangents())
			{
				CORE_LOG(HE_ERROR, "MeshImporter", "Failed to load mesh file with error: Mesh does not have required data");
				return SkyboxResourceLayout();
			}

			// Define layout per vertex in the mesh.
			for (i32 vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++)
			{
				SkyboxVertex vertexData;

				// Get the vertex data from the mesh via Assimp.
				vertexData.Position = Vec3f(mesh->mVertices[vertexIndex].x, mesh->mVertices[vertexIndex].y, mesh->mVertices[vertexIndex].z);

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
			mainLayout.Layout = subMeshLayout;
		}
		assimpImporter.FreeScene();
		return mainLayout;
	}

	TextureResourceLayout ResourceImporter::ImportTexture(String path)
	{
		i32 width, height, channels;
		byte* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		TextureResourceLayout layout;
		layout.ImageSize = Vec2i(width, height);
		layout.ImageData = MemoryOwnedBuffer(data, width * height * STBI_rgb_alpha);
		layout.Channels = channels;

		return layout;
	}

	MaterialResourceLayout ResourceImporter::ImportMaterial(String path)
	{
		if (!PlatformFile::Exists(path))
		{
			CORE_LOG(HE_ERROR, "MeshImporter", "Failed to load mesh file with error: File does not exist");
			return MaterialResourceLayout();
		}

		Assimp::Importer assimpImporter;

		const aiScene* scene = assimpImporter.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace | aiProcess_GenNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CORE_LOG(HE_ERROR, "MeshImporter", "Failed to load mesh file with error: %s", assimpImporter.GetErrorString());
			return MaterialResourceLayout();
		}

		String refreshPath = path.substr(0, path.find_last_of('/'));
		refreshPath += "/";

		MaterialResourceLayout mainLayout;

		if (scene->HasMaterials())
		{
			for (i32 i = 0; i < scene->mNumMaterials; i++)
			{
				SubMeshMaterial subMeshMaterial;
				subMeshMaterial.MaterialIndex = i;

				aiMaterial* material = scene->mMaterials[i];

				if(material->GetTextureCount(aiTextureType_BASE_COLOR) > 0)
				{
					// BASE COLOR TEXTURE IMPORT
					aiString textureName;
					material->GetTexture(aiTextureType_BASE_COLOR, 0, &textureName);

					String texturePath = refreshPath + textureName.C_Str();

					if (textureName.length > 0)
						subMeshMaterial.BaseTexture = ImportTexture(texturePath);
				}
				
				if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
				{
					// NORMAL TEXTURE IMPORT
					aiString normalTextureName;
					material->GetTexture(aiTextureType_NORMALS, 0, &normalTextureName);

					String normalTexturePath = refreshPath + normalTextureName.C_Str();

					if (normalTextureName.length > 0)
						subMeshMaterial.NormalTexture = ImportTexture(normalTexturePath);
				}

				subMeshMaterial.MaterialIndex = i;
				mainLayout.SubMeshMaterials.push_back(subMeshMaterial);
			}
		}

		assimpImporter.FreeScene();
		return mainLayout;
	}
}