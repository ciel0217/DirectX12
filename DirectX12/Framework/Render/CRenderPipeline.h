#pragma once
#include "../LowLevel/DirectX12Helper.h"
#include <unordered_map>
#include <vector>
#include "../LowLevel/CommandContext.h"
//#include "../Resources/FrameResources.h"

class CameraRender;
class CGameObject;
class FrameResources;
class CLight;
class StructuredBuffer;
struct BufferView;

class CGeometryRenderPipeline
{
protected:
	std::unordered_map<std::string, std::shared_ptr<FrameResources>> m_TextureResourece;

public:
	CGeometryRenderPipeline(){}
	virtual ~CGeometryRenderPipeline(){}

	virtual void SetUpRender() = 0;
	virtual void UninitRender() = 0;
	virtual void Draw(std::list<std::shared_ptr<CGameObject >> gameObjects[], CameraRender* cameraRender, CommandListSet& commandListSet) = 0;


	const std::unordered_map<std::string, std::shared_ptr<FrameResources>>& GetTextureResources() { return m_TextureResourece; }
};

class CLightRenderPipeline
{
protected:
	std::shared_ptr<StructuredBuffer> m_LightStructuredBuffer;
	std::shared_ptr<BufferView> m_LightView;

	const UINT MAX_LIGHT = 10;
	

public:
	CLightRenderPipeline(){}
	virtual ~CLightRenderPipeline(){}
	
	virtual void SetUpRender() = 0;
	virtual void UninitRender() = 0;
	virtual void Draw(std::unordered_map<std::string, std::shared_ptr<FrameResources>>& textureResources, 
		std::vector<std::shared_ptr<CLight>> lights, CommandListSet &commandListSet) = 0;
};