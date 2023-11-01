#pragma once
#include "../LowLevel/DirectX12Helper.h"
#include "../Resources/Texture2D.h"
#include "../Resources/Material.h"

class CRenderPipeline
{
protected:
	//std::unordered_map<std::string name, std::shared_ptr<TextureSet>> m_TextureResource;

public:
	CRenderPipeline(){}
	virtual ~CRenderPipeline(){}

	virtual void SetUpRender() = 0;
	virtual void UninitRender() = 0;
	virtual void Draw() = 0;
};