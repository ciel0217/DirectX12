#include "MeshRender.h"
#include "../Resources/MaterialManager.h"
#include "../LowLevel/Dx12GraphicsDevice.h"
#include "../LowLevel/DescriptorHeap.h"
#include "../Resources/CGameObject.h"
#include "../Resources/Model.h"

void MeshRender::SetUpRender()
{
	ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();

	m_WVPCBuffer.reset(new ConstantBuffer());
	m_WVPCBuffer->CreateConstantBuffer(device, sizeof(WVP));
	
	m_WVPCView.reset(new BufferView());
	DescriptorHeapManager::Intance().CreateConstantBufferView(m_WVPCBuffer->GetResource().GetAddressOf(), m_WVPCView.get(), 1);

}

void MeshRender::Draw()
{
	std::shared_ptr<Model> model = m_Self->GetModel();
	if (!model)
		return;


	Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();
	auto commandListSet = dxDevice->GetGraphicContext()->RequestCommandListSet();

	//size�݂͂�Ȉꏏ
	std::vector<VertexBuffer> vBuffer = model->GetVertexBuffer();
	std::vector<IndexBuffer> iBuffer = model->GetIndexBuffer();
	std::vector<UINT> indexNum = model->GetIndexNum();
	
	for (int i = 0; i < vBuffer.size(); i++)
	{
		VertexBuffer vB = vBuffer[i];
		IndexBuffer iB = iBuffer[i];
		UINT iNum = indexNum[i];

		commandListSet.m_CommandList.Get()->IASetVertexBuffers(0, 1, &vB.GetVertexBufferView());
		commandListSet.m_CommandList.Get()->IASetIndexBuffer(&iB.GetIndexBufferView());

		commandListSet.m_CommandList.Get()->DrawIndexedInstanced(iNum, 1, 0, 0, 0);
	}
	
}
