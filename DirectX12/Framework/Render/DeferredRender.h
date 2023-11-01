#pragma once
#include "CRenderPipeline.h"
#include "../Resources/Texture2D.h"
#include <unordered_map>


class DeferredRender : public CRenderPipeline
{
private:
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_TextureResourece;
	UINT m_ResoureceMax;

	void CalcTextureResource(std::string name);
public:
	DeferredRender(){}
	~DeferredRender(){}
	void SetUpRender()override;
	void UninitRender()override;
	void Draw()override;
};