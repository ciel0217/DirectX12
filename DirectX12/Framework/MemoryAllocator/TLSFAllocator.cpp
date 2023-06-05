#include "TLSFAllocator.h"

void TLSFAllocator::ShutDown()
{
}

void * TLSFAllocator::DivideMemory(unsigned int block_size)
{
	//block_num�̍ŏ��1Bit���擾
	unsigned int divide_bit = HigherBit(block_size);

	unsigned int mask = (1 << divide_bit) - 1;

	const unsigned int rs = mask - 5;

	unsigned int index = (divide_bit & mask) >> rs;

	if (m_FreeList[divide_bit * 32 + index]->m_Size == block_size)
	{

	}
	

	//�ŏ��1Bit���獶����1�Ŗ��߂�
	unsigned int my_bit = ~(FillRightBit(divide_bit) >> 1);
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
