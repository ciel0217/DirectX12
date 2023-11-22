#include "TLSFAllocator.h"

void TLSFAllocator::ShutDown()
{
	delete[] m_DataPtr;
	delete[] m_BlockPtr;
	
	m_FreeList.clear();
}

void TLSFAllocator::Init(unsigned int allocate_block_num)
{
	//引数の値を一番近い二の累乗に計算
	allocate_block_num--;
	unsigned int log2 = FastLog2(allocate_block_num);
	allocate_block_num = 1U << log2 + 1;

	m_AllocateBlockSize = allocate_block_num * (unsigned int)BLOCK_AND_HEADER_SIZE;
	m_AllocateDataSize = allocate_block_num * (unsigned int)BLOCK_DATA_SIZE;

	m_DataPtr = new unsigned char[m_AllocateDataSize];
	m_BlockPtr = new unsigned char[m_AllocateBlockSize];

	memset(m_DataPtr, 0, m_AllocateDataSize);
	memset(m_BlockPtr, 0, m_AllocateBlockSize);

	m_FreeList.resize(FREE_LIST_DIVISIONS * FREE_LIST_DIVISIONS);

	Block* block = new(m_BlockPtr)Block();
	block->m_Size = allocate_block_num;

	m_SLIFreeFlags.resize(FREE_LIST_DIVISIONS);

	RegistFreeList(block);
}

//引数
//欲しいメモリブロック数(メモリ数じゃないよ！)
void * TLSFAllocator::DivideMemory(unsigned int block_num)
{
	//block_numの最上位1BitのIndexを取得
	unsigned int fli = GetFLI(block_num);

	//block_numがおかしいからnullptr
	if (fli == -1)
		return nullptr;

	unsigned int sli = GetSLI(fli, block_num);

	unsigned int regist_index = fli * FREE_LIST_DIVISIONS + sli;

	Block* block = m_FreeList[regist_index];

	if (!block)
	{

		unsigned int sli_bit = CalcEnableSLIBit(fli, sli);
		unsigned int fli_bit = fli;

		if (sli_bit == 0)
		{
			//最大31回
			do
			{
				unsigned int fli_mask_bit = 0xffffffff << fli_bit;
				unsigned int fli_enable_bit = m_FLIFreeFlags & fli_mask_bit;

				//空いている領域が完全に0だったら終わり
				if (fli_enable_bit == 0)
					return nullptr;

				unsigned long index = 0;
				_BitScanForward(&index, fli_enable_bit);
				fli_bit = index;

				unsigned int sli_enable_bit = CalcEnableSLIBit(fli_bit, 0);

				//フリーリストあり
				if (sli_enable_bit != 0)
				{
					sli_bit = sli_enable_bit;
					break;
				}

				//フリーリストがない場合は次のfliに更新
			} while (1);
		}
		unsigned int sli_index = FastLog2(sli_bit);

		regist_index = fli_bit * FREE_LIST_DIVISIONS + sli_index;
		block = m_FreeList[regist_index];
	}

	RemoveFreeList(block);

	if (block->m_Size == block_num)
	{
		block->m_Enable = true;
		return m_DataPtr + BlockLocalIndex(block) * BLOCK_DATA_SIZE;
	}

	block->SetSize(block->m_Size - block_num);
	unsigned char* new_block_ptr = (unsigned char*)block + block->m_Size * BLOCK_AND_HEADER_SIZE;
	Block* new_block = new(new_block_ptr)Block();
	new_block->SetSize(block_num);
	new_block->m_Enable = true;


	RegistFreeList(block);

	return m_DataPtr + BlockLocalIndex(new_block) * BLOCK_DATA_SIZE;

}

void TLSFAllocator::ReleaseMemory(void * ptr)
{
	unsigned int index = DataLocalIndex(ptr);
	Block* block = (Block*)(m_BlockPtr + index * BLOCK_AND_HEADER_SIZE);

	//前後左右のブロックポインタ
	unsigned int* prev_block_size = (unsigned int*)((unsigned char*)block - sizeof(unsigned int));
	Block* prev_block = (Block*)((unsigned char*)block - *prev_block_size * BLOCK_AND_HEADER_SIZE);
	Block* nextBlock = (Block*)((unsigned char*)block + block->m_Size * BLOCK_AND_HEADER_SIZE);

	//配列の０番目なら前のブロックは存在しない
	if (index == 0) 
		prev_block = nullptr;
	

	//配列の最後なら次のブロックは存在しない
	if (index == (m_AllocateBlockSize / BLOCK_AND_HEADER_SIZE - block->m_Size))
		nextBlock = nullptr;
	

	//debugRemoveActiveList(block);

	//前マージ
	if (prev_block != nullptr) 
	{
		if (!prev_block->m_Enable) 
		{
			RemoveFreeList(prev_block);
			prev_block->SetSize(prev_block->m_Size + block->m_Size);

			block = prev_block;
			
		}
	}

	//後マージ
	if (nextBlock != nullptr) 
	{
		if (!nextBlock->m_Enable) 
		{
			RemoveFreeList(nextBlock);
			block->SetSize(block->m_Size + nextBlock->m_Size);
			
		}
	}

	//フリーリストに登録
	block->m_Enable = false;
	RegistFreeList(block);
}

void TLSFAllocator::RegistFreeList(Block * block)
{
	unsigned int fli_index = GetFLI(block->m_Size);
	unsigned int sli_index = GetSLI(fli_index, block->m_Size);
	unsigned int regist_index = fli_index * FREE_LIST_DIVISIONS + sli_index;

	if (m_FreeList[regist_index] == nullptr)
	{
		m_FreeList[regist_index] = block;

		m_FLIFreeFlags |= 1U << fli_index;
		m_SLIFreeFlags[fli_index] |= 1U << sli_index;
	}
	else 
	{
		m_FreeList[regist_index]->Regist(block);
	}
}

void TLSFAllocator::RemoveFreeList(Block * block)
{
	unsigned int fli_index = GetFLI(block->m_Size);
	unsigned int sli_index = GetSLI(fli_index, block->m_Size);

	unsigned int remove_index = fli_index * FREE_LIST_DIVISIONS + sli_index;

	block->remove();
	if (m_FreeList[remove_index] == block)
	{
		m_FreeList[remove_index] = block->m_ListNext;
	}

	if (m_FreeList[remove_index] == nullptr)
	{
		
		m_SLIFreeFlags[fli_index] &= ~(1U << sli_index);
		if (m_SLIFreeFlags[fli_index] == 0)
			m_FLIFreeFlags &= ~(1U << fli_index);
	}
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

unsigned int TLSFAllocator::CalcEnableSLIBit(unsigned int index, unsigned int sli_bit)
{
	unsigned int sli_mask_bit = 0xffffffff << sli_bit;
	unsigned int sli_enable_bit = m_SLIFreeFlags[index] & sli_mask_bit;

	return sli_enable_bit;
}
