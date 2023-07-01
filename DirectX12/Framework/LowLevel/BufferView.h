#include "DirectX12.h"

struct BufferView {
	BufferView() :cpuHandle(), gpuHandle(), location(0), size(0) {}
	virtual ~BufferView() {}

	inline bool isEnable() const { return cpuHandle.ptr != 0 && gpuHandle.ptr != 0 && location != 0 && size != 0; }

	/*RefBufferView getRefBufferView() const {
		return RefBufferView(gpuHandle);
	}*/

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	unsigned int location;
	unsigned int size;
};