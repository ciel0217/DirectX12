#pragma once
#include "../LowLevel/DirectX12Helper.h"
#include "../LowLevel/CommandContext.h"
#include "../LowLevel/BufferView.h"

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
	void CreateTexture(const ComPtr<ID3D12Device> &device, CommandContext* const context, ComPtr<ID3D12Resource> &uploadHeap, const std::string &name);
	void CreateTexture(const ComPtr<ID3D12Device> &device, CommandContext* const context, D3D12_RESOURCE_DESC resDesc);
	void ShutDown();

	const ComPtr<ID3D12Resource>& GetResource()const{ return m_TextureResource; }
};

struct TextureSet
{
	std::shared_ptr<Texture2D> TextureResource;
	std::shared_ptr<BufferView> TextureView;

	TextureSet(Texture2D* texRes, BufferView* texView)
	{
		TextureResource.reset(texRes);
		TextureView.reset(texView);
	}
};