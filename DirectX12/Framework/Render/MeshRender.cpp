#include "MeshRender.h"
#include "../Resources/MaterialManager.h"
#include "../LowLevel/Dx12GraphicsDevice.h"
#include "../LowLevel/DescriptorHeap.h"
#include "../Resources/CGameObject.h"
#include "../Resources/Model.h"

void MeshRender::SetUpRender()
{
	ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();

	m_WorldCBuffer.reset(new ConstantBuffer());
	m_WorldCBuffer->CreateConstantBuffer(device, sizeof(WorldMatrix));
	
	m_WorldView.reset(new BufferView());
	DescriptorHeapManager::Intance().CreateConstantBufferView(m_WorldCBuffer->GetResource().GetAddressOf(), m_WorldView.get(), 1);

}

void MeshRender::Draw()
{
	if (!m_Model)
		return;


	Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();
	auto commandListSet = dxDevice->GetGraphicContext()->RequestCommandListSet();

	//sizeはみんな一緒
	std::vector<VertexBuffer> vBuffer = m_Model->GetVertexBuffer();
	std::vector<IndexBuffer> iBuffer = m_Model->GetIndexBuffer();
	std::vector<UINT> indexNum = m_Model->GetIndexNum();

	//TODO::World行列計算
	//TODO::定数バッファの設定
	
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
