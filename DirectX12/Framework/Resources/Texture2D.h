#pragma once
#include "../LowLevel/DirectX12Helper.h"

struct DepthInfo
{
	DepthInfo(){}
	DepthInfo(unsigned int width, unsigned int height, DXGI_FORMAT format)
		: m_Width(width),m_Height(height), m_Format(format) {}

	unsigned int m_Width;
	unsigned int m_Height;
	DXGI_FORMAT m_Format;
};


class Texture2D
{
private:
	ComPtr<ID3D12Resource> m_TextureResource;

public:
	Texture2D(){}
	~Texture2D() { ShutDown(); }

	void CreateFromBackBuffer(const ComPtr<IDXGISwapChain3> &swapChain, UINT index);
	void CreateDepth(const ComPtr<ID3D12Device> &device, const DepthInfo info);
	void ShutDown();

	const ComPtr<ID3D12Resource>& GetResource()const{ return m_TextureResource; }
};