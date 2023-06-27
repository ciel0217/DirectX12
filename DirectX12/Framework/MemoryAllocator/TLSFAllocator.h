#pragma once
#include <vector>

struct Block {
	Block() : 
		m_Size(0), 
		m_ListPrev(nullptr), 
		m_ListNext(nullptr), 
		m_Enable(false) 
	{}

	void Regist(Block* block) 
	{
		if (m_ListNext != nullptr) 
			m_ListNext->m_ListPrev = block;
		
		block->m_ListNext = m_ListNext;
		block->m_ListPrev = this;

		m_ListNext = block;
	}

	void remove()
	{
		if (m_ListNext != nullptr) 
			m_ListNext->m_ListPrev = m_ListPrev;
		

		if (m_ListPrev != nullptr) 
			m_ListPrev->m_ListNext = m_ListNext;
		
	}

	//このブロックと終端タグにサイズ数を書き込み
	void SetSize(unsigned int size) 
	{
		this->m_Size = size;
		unsigned int* endHeaderPtr = (unsigned int*)((unsigned char*)this + (sizeof(Block) + sizeof(unsigned char)) * size);
		endHeaderPtr -= 1;
		*endHeaderPtr = size;
	}

	Block* m_ListPrev;
	Block* m_ListNext;
	unsigned int m_Size;
	bool m_Enable;
};

constexpr int tab32[32] = {
	0, 9, 1, 10, 13, 21, 2, 29,
	11, 14, 16, 18, 22, 25, 3, 30,
	8, 12, 20, 28, 15, 17, 24, 7,
	19, 27, 23, 6, 26, 5, 4, 31 };


inline constexpr bool IsPowOfTwo(unsigned int value) {
	return (value & (value - 1)) == 0;
}

inline constexpr int LowerBit(int bit) {
	return bit & -bit;
}

inline constexpr unsigned int FillRightBit(unsigned int bit) {
	bit |= bit >> 1;
	bit |= bit >> 2;
	bit |= bit >> 4;
	bit |= bit >> 8;
	bit |= bit >> 16;
	return bit;
}

inline unsigned int HigherBit(unsigned int bit) {
	unsigned int result = bit;
	result--;
	result = FillRightBit(result);
	result++;

	if (!IsPowOfTwo(bit)) {
		result >>= 1;
	}

	return result;
}


inline constexpr unsigned int FastLog2(unsigned int value) {
	value = FillRightBit(value);
	
	return tab32[(uint32_t)(value * 0x07C4ACDD) >> 27];
}

inline constexpr unsigned int Fassss(unsigned int value) {
	if (IsPowOfTwo(value)) {
		return FastLog2(value);
	}
}

constexpr unsigned char BLOCK_AND_HEADER_SIZE = sizeof(Block) + sizeof(unsigned char);

class TLSFAllocator
{
public:
	TLSFAllocator(unsigned int data_size) : BLOCK_DATA_SIZE(data_size) {};
	~TLSFAllocator() { ShutDown(); }

	void ShutDown();

	void* DivideMemory(unsigned int block_num);
	void ReleaseMemory(void* ptr);
	void RegistFreeList(Block* block);
	void RemoveFreeList(Block* block);

	//データポインタからブロックヘッダーを取得する
	inline Block* GetBlockFromDataPtr(void* dataPtr) 
	{
		return (Block*)(m_BlockPtr + dataLocalIndex(dataPtr) * BLOCK_AND_HEADER_SIZE);
	}

	//basePtrを開始としたデータ本体のローカルポインタを返す
	inline unsigned int dataLocalIndex(void* blockPtr) 
	{
		unsigned int local_ptr = reinterpret_cast<unsigned int>(blockPtr) - reinterpret_cast<unsigned int>(m_DataPtr);
		return local_ptr / BLOCK_DATA_SIZE;
	}

	//basePtrを開始としたブロックのローカルポインタを返す
	inline unsigned int blockLocalIndex(void* blockPtr) 
	{
		unsigned int local_ptr = reinterpret_cast<unsigned int>(blockPtr) - reinterpret_cast<unsigned int>(this->m_BlockPtr);
		return local_ptr / BLOCK_AND_HEADER_SIZE;
	}

	//ブロックローカルインデックスからアロケーターのデータポインタを求める
	void* getDataPtrFromBlockLocation(unsigned int blockLocation)
	{
		unsigned int local_ptr = blockLocation * BLOCK_DATA_SIZE;
		return m_DataPtr + local_ptr;
	}

private:
	unsigned int GetFLI(unsigned int num);
	unsigned int GetSLI(unsigned int fli, unsigned int size);

	const unsigned int FREE_LIST_DIVISIONS = 16;
	const unsigned int POWER_OF_TWO_FREE_LIST_DIVISIONS = FastLog2(FREE_LIST_DIVISIONS);
	
	const unsigned int BLOCK_DATA_SIZE;

	unsigned char* m_DataPtr;
	unsigned char* m_BlockPtr;
	unsigned int m_AllocateBlockSize;
	unsigned int m_AllocateDataSize;

	std::vector<Block*> m_FreeList;
	std::vector<unsigned int> m_SLIFreeFlags;
	unsigned int m_FLIFreeFlags;
};