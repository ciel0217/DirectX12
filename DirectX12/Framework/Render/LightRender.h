#pragma once
#include "CRenderPipeline.h"

class LightRender : public CLightRenderPipeline
{


public:
	LightRender() {}
	~LightRender() {}

	void SetUpRender() override;
	void UninitRender() override;
	void Draw(std::unordered_map<std::string, std::shared_ptr<FrameResources>>& textureResources,
		std::vector<std::shared_ptr<CLight>> lights, CommandListSet &commandListSet)override;
};