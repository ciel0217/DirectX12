#pragma once
#include <iostream>
#include "DirectX12.h"

inline std::string HrToString(HRESULT hr) {
	char str[64] = {};
	printf_s(str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	OutputDebugString(str);
	return std::string(str);
}


class HrException :public std::runtime_error {
public:
	HrException(HRESULT hr) :std::runtime_error(HrToString(hr)), _hr(hr) {}
	HRESULT error() const { return _hr; }
private:
	const HRESULT _hr;
};

inline void ThrowIfFailed(HRESULT hr) {
	if (FAILED(hr)) {
		throw HrException(hr);
	}
}