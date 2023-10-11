#include "MeshRender.h"
#include "../Resources/MaterialManager.h"
#include "../LowLevel/Dx12GraphicsDevice.h"

void MeshRender::Draw()
{
	ComPtr<ID3D12Device> device = Dx12GraphicsDevice::GetInstance()->GetDevice();
	RootSignature* currentRootSignature = MaterialManager::GetInstance()->GetRenderSet(m_Material->GetMaterialName())->rootSignature;

}
