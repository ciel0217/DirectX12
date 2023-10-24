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
	DescriptorHeapManager::Instance().CreateConstantBufferView(m_WorldCBuffer->GetResource().GetAddressOf(), m_WorldView.get(), 1);

}

void MeshRender::Draw()
{
	if (!m_Model)
		return;

	Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();
	auto commandListSet = dxDevice->GetGraphicContext()->RequestCommandListSet();

	//sizeÇÕÇ›ÇÒÇ»àÍèè
	std::vector<VertexBuffer> vBuffer = m_Model->GetVertexBuffer();
	std::vector<IndexBuffer> iBuffer = m_Model->GetIndexBuffer();
	std::vector<UINT> indexNum = m_Model->GetIndexNum();

	//TODO::WorldçsóÒåvéZ(âÒì]ÅAägëÂñ¢ëŒâû)
	WorldMatrix worldMat;
	worldMat.w = XMMatrixTranslation(m_Self->GetPosition().x, m_Self->GetPosition().y, m_Self->GetPosition().z);
	worldMat.w = XMMatrixTranspose(worldMat.w);
	
	m_WorldCBuffer->WriteData(&worldMat, sizeof(WorldMatrix));

	m_Material->GetRenderSet()->rootSignature->SetGraphicsRootDescriptorTable(commandListSet, "World", m_WorldView);


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
