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

	void remove() {
		if (m_ListNext != nullptr) 
			m_ListNext->m_ListPrev = m_ListPrev;
		

		if (m_ListPrev != nullptr) 
			m_ListPrev->m_ListNext = m_ListNext;
		
	}

	//このブロックと終端タグにサイズ数を書き込み
	void SetSize(unsigned int size) {
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


class TLSFAllocator
{
public:
	TLSFAllocator(unsigned int data_size) : BLOCK_DATA_SIZE(data_size) {};

private:
	const unsigned int BLOCK_DATA_SIZE;
};