#include "TLSFAllocator.h"

void TLSFAllocator::ShutDown()
{
}

void * TLSFAllocator::DivideMemory(unsigned int block_num)
{
	//block_num�̍ŏ��1Bit���擾
	unsigned int divide_bit = HigherBit(block_num);
	//�ŏ��1Bit���獶����1�Ŗ��߂�
	unsigned int mask = ~(FillRightBit(divide_bit) >> 1);
	//FreeFlags��mask�ŁA�󂢂Ă���̈���擾
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
