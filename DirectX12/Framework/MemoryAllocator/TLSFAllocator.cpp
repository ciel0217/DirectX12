#include "TLSFAllocator.h"

void TLSFAllocator::ShutDown()
{
}

//����
//�~�����������u���b�N��(������������Ȃ���I)
void * TLSFAllocator::DivideMemory(unsigned int block_num)
{
	//block_num�̍ŏ��1Bit��Index���擾
	unsigned int fli = GetFLI(block_num);

	//block_num��������������nullptr
	if (fli == -1)
		return nullptr;

	unsigned int sli = GetSLI(fli, block_num);

	Block* block = m_FreeList[fli * FREE_LIST_DIVISIONS + sli];

	if (!block)
	{
		unsigned int fli_bit =  fli;
		unsigned int sli_bit = sli;
		unsigned long index = fli;
		/*unsigned int fli_enable_bit = m_FLIFreeFlags & fli_bit;
		
		_BitScanForward(&index, fli_enable_bit);*/

		//�ő�31��
		do 
		{
			unsigned int sli_mask_bit = 0xffffffff << sli_bit;
			unsigned int sli_enable_bit = m_SLIFreeFlags[index] & sli_mask_bit;

			//�t���[���X�g����
			if (sli_enable_bit != 0)
				break;

			//�t���[���X�g���Ȃ��ꍇ�͎���fli�ɍX�V
			sli_bit = 0;

			unsigned int fli_mask_bit = 0xffffffff << fli_bit;
			unsigned int fli_enable_bit = m_FLIFreeFlags & fli_mask_bit;

			//�󂢂Ă���̈悪���S��0��������I���
			if (fli_enable_bit == 0)
				return nullptr;
			
			_BitScanForward(&index, fli_enable_bit);
			fli_bit = index;
		} while (fli_bit != 0);
		
		block = m_FreeList[fli_bit * FREE_LIST_DIVISIONS + sli_bit];
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
