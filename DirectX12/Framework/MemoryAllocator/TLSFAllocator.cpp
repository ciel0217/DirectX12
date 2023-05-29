#include "TLSFAllocator.h"

void TLSFAllocator::ShutDown()
{
}

void * TLSFAllocator::DivideMemory(unsigned int block_num)
{
	unsigned int divide_bit = HigherBit(block_num);
	unsigned int mask = ~(FillRightBit(divide_bit) >> 1);
	unsigned int free_bit = (m_FreeFlags & mask);

	return nullptr;
}

void TLSFAllocator::ReleaseMemory(void * ptr)
{
}

void TLSFAllocator::RegistFreeList(Block * block)
{
}

void TLSFAllocator::RemoveFreeList(Block * block)
{
}
