#include "Model.h"
#include "../LowLevel/Dx12GraphicsDevice.h"
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "../LowLevel/DescriptorHeap.h"
#include "MaterialManager.h"

//TODO::animation未対応
std::vector<std::shared_ptr<Material>> Model::LoadModel(bool inverseU, bool inverseV)
{
	Assimp::Importer importer;

	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;

	auto scene = importer.ReadFile(m_Name, flag);
	
	LoadMesh(scene, inverseU, inverseV);
	LoadTexture(scene);

	return m_Materials;
}

void Model::CreateVertexBuffer(const std::vector<Mesh> &meshes)
{
	m_VertexBuffer.resize(meshes.size());

	ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();
	CommandContext* context = Dx12GraphicsDevice::GetInstance()->GetGraphicContext();
	
	for (UINT i = 0; i < meshes.size(); i++)
	{
		m_VertexBuffer[i].CreateVertexBuffer(device, context, meshes[i].m_Vertices);
	}
}

void Model::CreateIndexBuffer(const std::vector<Mesh> &meshes)
{
	m_IndexBuffer.resize(meshes.size());

	const ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();
	CommandContext* context = Dx12GraphicsDevice::GetInstance()->GetGraphicContext();
	ComPtr<ID3D12Resource> uploadHeap;
	auto set = context->RequestCommandListSet();
	ComPtr<ID3D12GraphicsCommandList> commandList = set.m_CommandList;

	for (UINT i = 0; i < meshes.size(); i++)
	{
		m_IndexBuffer[i].CreateIndexBuffer(device, commandList, uploadHeap, meshes[i].m_Indices);
	}

	context->ExecuteCommandList(commandList);
	context->DiscardCommandListSet(set);
	context->WaitForIdle();
}

void Model::CreateTexture(TextureType type, aiMaterial* material, std::unordered_map<std::string, std::shared_ptr<TextureSet>> &textures)
{
	aiTextureType textureType;

	std::string textureName;
	switch (type)
	{
	case eDiffuse:
		textureType = aiTextureType_DIFFUSE;
		textureName = "DiffuseTexture";
		break;
	case eNormal:
		textureType = aiTextureType_NORMALS;
		textureName = "NormalTexture";
		break;
	case eShiness:
		textureType = aiTextureType_SHININESS;
		textureName = "ShininessTexture";
		break;
	case eSpecular:
		textureType = aiTextureType_SPECULAR;
		textureName = "SpecularTexture";
		break;
	default:
		textureType = aiTextureType_NONE;
		textureName = "None";
		break;
	}

	ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();
	CommandContext* commandContext = Dx12GraphicsDevice::GetInstance()->GetGraphicContext();

	if (material->GetTextureCount(textureType) > 0)
	{
		aiString path;
		//とりあえず同じ種類のテクスチャは一つだけ
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path,
			NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::string fullPath = "Asset/Model/" + std::string(path.C_Str());

			ComPtr<ID3D12Resource> upLoadHeap;
			//これでだめだったら変える
			Texture2D* texture = new Texture2D();
			texture->CreateTexture(device, commandContext, upLoadHeap, fullPath);

			BufferView* bufferView = new BufferView();
			DescriptorHeapManager::Instance().CreateTextureShaderResourceView(texture->GetResource().GetAddressOf(), bufferView, 1);

			textures[textureName].reset(new TextureSet(texture, bufferView));
		}
	}
}

void Model::LoadMesh(const aiScene* scene, bool inverseU, bool inverseV)
{
	if (!scene)
		return;

	std::vector<Mesh> meshes;
	meshes.clear();
	meshes.resize(scene->mNumMeshes);
	m_IndexNum.resize(scene->mNumMeshes);

	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{
		const auto  mesh = scene->mMeshes[i];
		meshes[i].m_Vertices.resize(mesh->mNumVertices);
		meshes[i].m_Indices.resize(mesh->mNumFaces * 3);//1Faceは三角形だから
		m_IndexNum[i] = mesh->mNumFaces * 3;

		for (UINT j = 0; j < mesh->mNumVertices; j++)
		{
			auto position = mesh->mVertices[j];
			auto normal = mesh->mNormals[j];
			auto texCoord = (mesh->HasTextureCoords(0)) ?  (mesh->mTextureCoords[0][j]) : (aiVector3D(0.0f, 0.0f, 0.0f));
			auto color = (mesh->HasVertexColors(0)) ? (mesh->mColors[0][j]) : (aiColor4D(0.0f, 0.0f, 0.0f, 1.0f));

			meshes[i].m_Vertices[j].m_Position = XMFLOAT3(position.x, position.y, position.z);
			meshes[i].m_Vertices[j].m_Normal = XMFLOAT3(normal.x, normal.y, normal.z);
			meshes[i].m_Vertices[j].m_TexCoord = XMFLOAT2(texCoord.x, texCoord.y);
			meshes[i].m_Vertices[j].m_Diffuse = XMFLOAT4(color.r, color.g, color.b, color.a);

			if (inverseU)
				meshes[i].m_Vertices[j].m_TexCoord.x = 1.0 - meshes[i].m_Vertices[j].m_TexCoord.x;
			else if (inverseV)
				meshes[i].m_Vertices[j].m_TexCoord.y = 1.0 - meshes[i].m_Vertices[j].m_TexCoord.y;
		}

		for (UINT j = 0; j < mesh->mNumFaces; j++)
		{
			const auto & face = mesh->mFaces[j];

			meshes[i].m_Indices[j * 3 + 0] = face.mIndices[0];
			meshes[i].m_Indices[j * 3 + 1] = face.mIndices[1];
			meshes[i].m_Indices[j * 3 + 2] = face.mIndices[2];
		}
	}

	CreateVertexBuffer(meshes);
	CreateIndexBuffer(meshes);
}

void Model::LoadTexture(const aiScene * scene)
{
	if (!scene)
		return;

	UINT materialNums = scene->mNumMaterials;

	std::unordered_map<std::string, std::shared_ptr<TextureSet>> textures;

	m_Materials.resize(materialNums);

	std::string root_name = scene->mRootNode->mName.C_Str();

	for (int i = 0; i < materialNums; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		CreateTexture(eDiffuse, material, textures);

		CreateTexture(eNormal, material, textures);

		CreateTexture(eShiness, material, textures);

		CreateTexture(eSpecular, material, textures);

		std::shared_ptr<Material> myMaterial = 
		MaterialManager::GetInstance()->CreateMaterial(root_name + material->GetName().C_Str(), DEFAULT_VERTEX_SHADER_NAME, DEFAULT_PIXEL_SHADER_NAME, 
			MaterialManager::OPACITY_RENDER_QUEUE, textures);

		m_Materials[i] = myMaterial;
	}

	return;
}

