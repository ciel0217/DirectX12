#include "TLSFAllocator.h"

void TLSFAllocator::ShutDown()
{
}

//引数
//欲しいメモリブロック数(メモリ数じゃないよ！)
void * TLSFAllocator::DivideMemory(unsigned int block_num)
{
	//block_numの最上位1BitのIndexを取得
	unsigned int fli = GetFLI(block_num);
	unsigned int sli = GetSLI(fli, block_num);

	Block* block = m_FreeList[fli * FREE_LIST_DIVISIONS + sli];

	if (!block)
	{
		unsigned int my_bit = 0xffffffff << sli;
		unsigned int enable_bit = m_FreeFlags[fli] & my_bit;

		if (enable_bit == 0)
		{
			for (fli; fli < m_FreeFlags.size(); fli++)
			{
				my_bit = 0xffffffff << fli;
				enable_bit = m_FreeFlags[fli] & my_bit;

				if (enable_bit != 0)
					break;
			}

			//フリーリスト見つからない
			if (enable_bit == 0)
				return nullptr;
		}

		unsigned long index;
		_BitScanForward(&index, enable_bit);

		block = m_FreeList[fli * FREE_LIST_DIVISIONS + sli];

	}

	RemoveFreeList(block);

	if (block->m_Size == block_num)
	{
		block->m_Enable = true;
		return m_DataPtr + blockLocalIndex(block) * BLOCK_DATA_SIZE;
	}

	block->SetSize(block->m_Size - block_num);
	unsigned char* new_block_ptr = (unsigned char*)block + block->m_Size * BLOCK_AND_HEADER_SIZE;
	Block* new_block = new(new_block_ptr)Block();
	new_block->SetSize(block_num);
	new_block->m_Enable = true;

	RegistFreeList(block);

	return m_DataPtr + blockLocalIndex(new_block) * BLOCK_DATA_SIZE;

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

unsigned int TLSFAllocator::GetFLI(unsigned int num)
{
	if (num == 0)
		return -1;

	unsigned long index = 0;

	_BitScanReverse(&index, num);
	return index;
}

unsigned int TLSFAllocator::GetSLI(unsigned int fli, unsigned int size)
{
	const unsigned int mask = (1 << fli) - 1;

	const unsigned int rs = fli - POWER_OF_TWO_FREE_LIST_DIVISIONS;

	return (size & mask) >> rs;
}
