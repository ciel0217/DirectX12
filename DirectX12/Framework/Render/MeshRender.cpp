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

void MeshRender::Draw(CommandListSet* commandListSet, UINT materialIndex)
{

	Dx12GraphicsDevice* dxDevice = Dx12GraphicsDevice::GetInstance();
	//auto commandListSet = dxDevice->GetGraphicContext()->RequestCommandListSet();
	//sizeはみんな一緒
	std::vector<VertexBuffer> vBuffer = m_Model->GetVertexBuffer();
	std::vector<IndexBuffer> iBuffer = m_Model->GetIndexBuffer();
	std::vector<UINT> indexNum = m_Model->GetIndexNum();

	//TODO::World行列計算(回転、拡大未対応)
	WorldMatrix worldMat;
	worldMat.w = XMMatrixTranslation(m_Self->GetPosition().x, m_Self->GetPosition().y, m_Self->GetPosition().z);
	worldMat.w = XMMatrixTranspose(worldMat.w);
	
	m_WorldCBuffer->WriteData(&worldMat, sizeof(WorldMatrix));

	std::shared_ptr<Material> material = m_Materials[materialIndex];
	std::unordered_map<std::string, std::shared_ptr<TextureSet>> textures = material->GetTextures();
	std::unordered_map<std::string, std::shared_ptr<CBufferSet>> cBuffers = material->GetCBuffers();
	//固定
	material->GetRenderSet()->rootSignature->SetGraphicsRootDescriptorTable(commandListSet, "World", m_WorldView);

	//テクスチャセット
	for (auto tex : textures)
	{
		material->GetRenderSet()->rootSignature->SetGraphicsRootDescriptorTable(commandListSet, tex.first, tex.second->TextureView);
	}

	//定数バッファセット
	for (auto cBuf : cBuffers)
	{
		if (cBuf.first == "VP" || cBuf.first == "World")
			continue;

		material->GetRenderSet()->rootSignature->SetGraphicsRootDescriptorTable(commandListSet, cBuf.first, cBuf.second->constantBufferView);
	}

	for (int i = 0; i < vBuffer.size(); i++)
	{
		VertexBuffer vB = vBuffer[i];
		IndexBuffer iB = iBuffer[i];
		UINT iNum = indexNum[i];

		commandListSet->m_CommandList.Get()->IASetVertexBuffers(0, 1, &vB.GetVertexBufferView());
		commandListSet->m_CommandList.Get()->IASetIndexBuffer(&iB.GetIndexBufferView());


		commandListSet->m_CommandList.Get()->DrawIndexedInstanced(iNum, 1, 0, 0, 0);
	}
	
}
