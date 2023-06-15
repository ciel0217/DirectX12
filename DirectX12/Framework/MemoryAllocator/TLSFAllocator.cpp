#include "TLSFAllocator.h"

void TLSFAllocator::ShutDown()
{
}

void * TLSFAllocator::DivideMemory(unsigned int block_size)
{
	//block_sizeの最上位1Bitを取得
	unsigned int fli = HigherBit(block_size);

	unsigned int mask = (1 << fli) - 1;

	const unsigned int rs = fli - POWER_OF_TWO_FREE_LIST_DIVISIONS;

	unsigned int index = (fli & mask) >> rs;

	//最上位1Bitから左側を1で埋める
	unsigned int my_bit = ~(FillRightBit(fli) >> 1);
	//FreeFlagsとmaskで、空いている領域を取得
	unsigned int free_bit = (m_FreeFlags & my_bit);




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
