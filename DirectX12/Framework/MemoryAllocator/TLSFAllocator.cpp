#include "TLSFAllocator.h"

void TLSFAllocator::ShutDown()
{
}

void * TLSFAllocator::DivideMemory(unsigned int block_num)
{
	//block_num‚ÌÅãˆÊ1Bit‚ğæ“¾
	unsigned int divide_bit = HigherBit(block_num);
	//ÅãˆÊ1Bit‚©‚ç¶‘¤‚ğ1‚Å–„‚ß‚é
	unsigned int mask = ~(FillRightBit(divide_bit) >> 1);
	//FreeFlags‚Æmask‚ÅA‹ó‚¢‚Ä‚¢‚é—Ìˆæ‚ğæ“¾
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
