#pragma once
#include "../LowLevel/DirectX12Helper.h"

// 頂点構造体
struct VERTEX_3D
{
	XMFLOAT3 m_Position;//座標
	XMFLOAT3 m_Normal;//法線
	XMFLOAT2 m_TexCoord;//UV値
	XMFLOAT4 m_Diffuse;//頂点カラー

	//D3DXVECTOR3 Tangent;//tangent(接ベクトル？なんていうか分からんw)
	//D3DXVECTOR3 Binormal;//従法線

	//float Dummy[2];
};

class CommandContext;

//TODO:::::もっとクラス分割できそう
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
	//GPUの仮想アドレスを取得
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() const { return m_Resource.Get()->GetGPUVirtualAddress();}
	//TODO:::Resourceの解放するべきか
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
	//GPUの仮想アドレスを取得
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() const { return m_Resource.Get()->GetGPUVirtualAddress(); }
	//TODO:::Resourceの解放するべきか
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

