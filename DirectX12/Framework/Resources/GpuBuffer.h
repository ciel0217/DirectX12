#pragma once
#include "../LowLevel/DirectX12Helper.h"

// ���_�\����
struct VERTEX_3D
{
	XMFLOAT3 m_Position;//���W
	XMFLOAT3 m_Normal;//�@��
	XMFLOAT2 m_TexCoord;//UV�l
	XMFLOAT4 m_Diffuse;//���_�J���[

	//D3DXVECTOR3 Tangent;//tangent(�ڃx�N�g���H�Ȃ�Ă������������w)
	//D3DXVECTOR3 Binormal;//�]�@��

	//float Dummy[2];
};

class CommandContext;

//TODO:::::�����ƃN���X�����ł�����
class GpuReadOnlyBuffer
{
protected:
	ComPtr<ID3D12Resource> m_Resource;

public:
	GpuReadOnlyBuffer(){}
	virtual ~GpuReadOnlyBuffer() { ShutDown(); }

	template<typename T>
	void CreateWithData(const ComPtr<ID3D12Device> &device, const ComPtr<ID3D12GraphicsCommandList> &commandList, const std::vector<T>& data);

	void CreateNonData(const ComPtr<ID3D12Device> &device, const UINT size, const D3D12_RESOURCE_FLAGS flags);

	const ComPtr<ID3D12Resource>& GetResource() { return m_Resource; }
	//GPU�̉��z�A�h���X���擾
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() const { return m_Resource.Get()->GetGPUVirtualAddress();}
	//TODO:::Resource�̉������ׂ���
	virtual void ShutDown(){}
};

class GpuAndCpuBuffer
{
protected:
	ComPtr<ID3D12Resource> m_Resource;
	void* m_DataPtr;

public:
	GpuAndCpuBuffer() {}
	virtual ~GpuAndCpuBuffer() { ShutDown(); }

	void CreateNonData(const ComPtr<ID3D12Device> &device, const UINT size);

	void WriteData(const void* data, const UINT size, const UINT startOffset = 0);

	const ComPtr<ID3D12Resource>& GetResource() { return m_Resource; }
	//GPU�̉��z�A�h���X���擾
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() const { return m_Resource.Get()->GetGPUVirtualAddress(); }
	//TODO:::Resource�̉������ׂ���
	virtual void ShutDown() {}
};


class VertexBuffer : public GpuReadOnlyBuffer
{
private:
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;

public:
	void CreateVertexBuffer(const ComPtr<ID3D12Device> &device, CommandContext* const context, const std::vector<VERTEX_3D>& data);
};

class IndexBuffer : public GpuReadOnlyBuffer
{
private:
	D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
	UINT m_IndexCount;

public:
	void CreateIndexBuffer(const ComPtr<ID3D12Device> &device, CommandContext* const context, const std::vector<UINT>& data);
};

class ConstantBuffer : public GpuAndCpuBuffer
{
private:
	UINT m_Size;

public:
	void CreateConstantBuffer(const ComPtr<ID3D12Device> &device, const UINT size);
};

