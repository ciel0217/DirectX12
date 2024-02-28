#include "LightRender.h"
#include "../LowLevel/BufferView.h"
#include "../Resources/GpuBuffer.h"

void LightRender::SetUpRender()
{
	m_LightCBuffer
}

void LightRender::UninitRender()
{
}

void LightRender::Draw(std::unordered_map<std::string, std::shared_ptr<FrameResources>>& textureResources, std::vector<std::shared_ptr<CLight>> lights, CommandListSet & commandListSet)
{
}
