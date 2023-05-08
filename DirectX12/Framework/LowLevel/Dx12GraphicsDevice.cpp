#include "Dx12GraphicsDevice.h"
#include "DirectX12Helper.h"

BOOL Dx12GraphicsDevice::Init()
{
	UINT _dxgiFactoryFlags = 0;
#ifdef _DEBUG
	ComPtr<ID3D12Debug> _debugController;
	
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController)))) 
	{
		_debugController->EnableDebugLayer();
		_dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	}
#endif

	ComPtr<IDXGIFactory4> _factory4;
	ThrowIfFailed(CreateDXGIFactory2(_dxgiFactoryFlags, IID_PPV_ARGS(&_factory4)));

	ComPtr<IDXGIAdapter1> _adapter1;
	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != _factory4->EnumAdapters1(adapterIndex, &_adapter1); adapterIndex++) 
	{
		DXGI_ADAPTER_DESC1 desc;
		_adapter1->GetDesc1(&desc);

		//仮想Direct3D12対応GPUは無視する
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) 
			continue;
		

		//デバイス生成の確認が成功したら終了
		if (SUCCEEDED(D3D12CreateDevice(_adapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))) 
			break;
		
	}

	//デバイス生成
	ThrowIfFailed(D3D12CreateDevice(_adapter1.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)));




	return 0;
}

BOOL Dx12GraphicsDevice::Release()
{
	return 0;
}
