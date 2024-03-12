#pragma once
#include "../LowLevel/DirectX12Helper.h"
#include <unordered_map>
#include <vector>
#include "../LowLevel/CommandContext.h"
#include "../Resources/Material.h"
//#include "../Resources/FrameResources.h"

class CameraRender;
class CGameObject;
class FrameResources;
class CLight;
class StructuredBuffer;
class CRender;
struct BufferView;

struct RenderingSet
{
	CRender* Render;
	Material* Mat;
	UINT MatIndex;

	RenderingSet(CRender* render, Material* material, UINT matIndex)
	{
		Render = render;
		Mat = material;
		MatIndex = matIndex;
	}

	bool operator < (RenderingSet* const rhs)const
	{
		if (Mat->GetRenderQueue() == rhs->Mat->GetRenderQueue())
			return Mat->GetMaterialName() < rhs->Mat->GetMaterialName();

		return Mat->GetRenderQueue() < rhs->Mat->GetRenderQueue();
	}
};


class CGeometryRenderPipeline
{
protected:
	std::unordered_map<std::string, std::shared_ptr<FrameResources>> m_TextureResourece;

public:
	CGeometryRenderPipeline(){}
	virtual ~CGeometryRenderPipeline(){}

	virtual void SetUpRender() = 0;
	virtual void UninitRender() = 0;
	virtual void Draw(std::list<std::shared_ptr<RenderingSet>> gameObjects, CameraRender* cameraRender, CommandListSet& commandListSet) = 0;


	const std::unordered_map<std::string, std::shared_ptr<FrameResources>>& GetTextureResources() { return m_TextureResourece; }
};

class CLightRenderPipeline
{
protected:
	std::unordered_map<std::string, std::shared_ptr<FrameResources>> m_TextureResourece;

	std::shared_ptr<StructuredBuffer> m_LightStructuredBuffer;
	std::shared_ptr<BufferView> m_LightView;
	std::shared_ptr<Material> m_RenderMat;

	const UINT MAX_LIGHT = 10;
	

public:
	CLightRenderPipeline(){}
	virtual ~CLightRenderPipeline(){}
	
	virtual void SetUpRender() = 0;
	virtual void UninitRender() = 0;
	virtual void Draw(std::unordered_map<std::string, std::shared_ptr<FrameResources>>& textureResources, 
		std::vector<std::shared_ptr<CLight>> lights, CommandListSet &commandListSet) = 0;

	const std::unordered_map<std::string, std::shared_ptr<FrameResources>>& GetTextureResources() { return m_TextureResourece; }
};