#pragma once
#include "DirectX12.h"


class DescriptorHeap
{
private:
	static DescriptorHeap* m_Instance;

public:
	DescriptorHeap() { if (!m_Instance)m_Instance = this; }
	~DescriptorHeap(){}

	

};