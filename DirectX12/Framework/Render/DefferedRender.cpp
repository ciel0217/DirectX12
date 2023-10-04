#include "DefferedRender.h"
#include "../Resources/Shader.h"
#include "../LowLevel/RootSignature.h"
#include "../LowLevel/PipelineStateObject.h"
#include "../LowLevel/Dx12GraphicsDevice.h"

void DefferedRender::SetUpRender()
{
	ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();

	m_VertexShader = new VertexShader();
	m_VertexShader->Create("");

	m_PixelShader = new PixelShader();
	m_PixelShader->Create("");

	m_RootSignature = new RootSignature();
	m_RootSignature->Create(device, m_VertexShader, m_PixelShader);
	
	m_PSO = new PipelineStateObject();
	m_PSO->CreateGraphicPipeline(device, m_RootSignature, m_VertexShader, m_PixelShader);
}

void DefferedRender::UninitRender()
{
	delete m_VertexShader;
	delete m_PixelShader;
	delete m_RootSignature;
	delete m_PSO;
}

void DefferedRender::Draw()
{
}
