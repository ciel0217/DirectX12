#include "TLSFAllocator.h"

void TLSFAllocator::ShutDown()
{
}

void * TLSFAllocator::DivideMemory(unsigned int block_size)
{
	//block_size‚ÌÅãˆÊ1Bit‚ğæ“¾
	unsigned int fli = HigherBit(block_size);

	unsigned int mask = (1 << fli) - 1;

	const unsigned int rs = fli - POWER_OF_TWO_FREE_LIST_DIVISIONS;

	unsigned int index = (fli & mask) >> rs;

	//ÅãˆÊ1Bit‚©‚ç¶‘¤‚ğ1‚Å–„‚ß‚é
	unsigned int my_bit = ~(FillRightBit(fli) >> 1);
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
