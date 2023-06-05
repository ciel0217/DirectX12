#include "TLSFAllocator.h"

void TLSFAllocator::ShutDown()
{
}

void * TLSFAllocator::DivideMemory(unsigned int block_size)
{
	//block_num‚ÌÅãˆÊ1Bit‚ğæ“¾
	unsigned int divide_bit = HigherBit(block_size);

	unsigned int mask = (1 << divide_bit) - 1;

	const unsigned int rs = mask - 5;

	unsigned int index = (divide_bit & mask) >> rs;

	if (m_FreeList[divide_bit * 32 + index]->m_Size == block_size)
	{

	}
	

	//ÅãˆÊ1Bit‚©‚ç¶‘¤‚ğ1‚Å–„‚ß‚é
	unsigned int my_bit = ~(FillRightBit(divide_bit) >> 1);
	//FreeFlags‚Æmask‚ÅA‹ó‚¢‚Ä‚¢‚é—Ìˆæ‚ğæ“¾
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
