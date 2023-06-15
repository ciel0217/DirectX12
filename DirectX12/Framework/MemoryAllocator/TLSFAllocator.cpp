#include "TLSFAllocator.h"

void TLSFAllocator::ShutDown()
{
}

void * TLSFAllocator::DivideMemory(unsigned int block_size)
{
	//block_size�̍ŏ��1Bit���擾
	unsigned int fli = HigherBit(block_size);

	unsigned int mask = (1 << fli) - 1;

	const unsigned int rs = fli - POWER_OF_TWO_FREE_LIST_DIVISIONS;

	unsigned int index = (fli & mask) >> rs;

	//�ŏ��1Bit���獶����1�Ŗ��߂�
	unsigned int my_bit = ~(FillRightBit(fli) >> 1);
	//FreeFlags��mask�ŁA�󂢂Ă���̈���擾
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
