#include "TLSFAllocator.h"

void TLSFAllocator::ShutDown()
{
}

void * TLSFAllocator::DivideMemory(unsigned int block_num)
{
	//block_numの最上位1Bitを取得
	unsigned int divide_bit = HigherBit(block_num);
	//最上位1Bitから左側を1で埋める
	unsigned int mask = ~(FillRightBit(divide_bit) >> 1);
	//FreeFlagsとmaskで、空いている領域を取得
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
