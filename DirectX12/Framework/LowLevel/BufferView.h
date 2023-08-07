#include "DirectX12.h"

struct BufferView {
	BufferView() :m_CpuHandle(), m_GpuHandle(), m_Location(0), m_Size(0) {}
	virtual ~BufferView() {}

	inline bool isEnable() const { return m_CpuHandle.ptr != 0 && m_GpuHandle.ptr != 0 && m_Location != 0 && m_Size != 0; }

	/*RefBufferView getRefBufferView() const {
		return RefBufferView(m_GpuHandle);
	}*/

	D3D12_CPU_DESCRIPTOR_HANDLE m_CpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_GpuHandle;
	unsigned int m_Location;
	unsigned int m_Size;
};