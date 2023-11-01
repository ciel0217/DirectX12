#include "DeferredRender.h"
#include "../Resources/Shader.h"
#include "../LowLevel/RootSignature.h"
#include "../LowLevel/PipelineStateObject.h"
#include "../LowLevel/Dx12GraphicsDevice.h"



/*
GBuffer - > LightŒvŽZ - > ”¼“§–¾ - > 2D
‚Ü‚Å‚Ìˆê˜A‚ðŽÀ‘•
*/

void DeferredRender::CalcTextureResource(std::string name)
{
	Texture2D* texture = new Texture2D();
	
}

void DeferredRender::SetUpRender()
{
	std::string resoureceName[] = {"BaseColor", "Normal", "Roughness", "Specular"};
	m_ResoureceMax = sizeof(resoureceName) / sizeof(resoureceName[0]);

	for (int i = 0; i < m_ResoureceMax; i++)
	{
		CalcTextureResource(resoureceName[i]);
	}
}

void DeferredRender::UninitRender()
{

}

void DeferredRender::Draw()
{
}
