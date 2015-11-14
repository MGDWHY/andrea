#pragma once

#include <vector>

class MEMemoryBlock;
class MEMemoryPool;

class MEMemoryBlock {
private:
	MEMemoryPool * pOwner;
	unsigned int pStartingChunk;
	unsigned int pChunksCount;
public:
	void* GetPointer();
	bool operator==(const MEMemoryBlock& other);
	unsigned int GetStartingChunk() { return pStartingChunk; }
	unsigned int GetChunksCount() { return pChunksCount; }
	friend class MEMemoryPool;
};

class MEMemoryPool
{
private:
	
	struct MEChunkInfo {
		bool Free;
		MEChunkInfo() { Free = true; }
	};

	MEChunkInfo * pChunks;
	char * pData;
	unsigned int pTotalSize;
	unsigned int pChunkSize;
	unsigned int pNumChunks;
	unsigned int pFreeChunks;

	void Reallocate(unsigned int newChunks);
	unsigned int CalculateNeededChunks(unsigned int size);
	bool FindFreeBlock(unsigned int chunks, unsigned int& startingChunk);

public:

	MEMemoryPool(unsigned int preferredSize, unsigned int chunkSize);
	~MEMemoryPool(void);

	unsigned int GetTotalSize() { return pTotalSize; }
	unsigned int GetChunkSize() { return pChunkSize; }
	unsigned int GetChunksCount() { return pNumChunks; }
	unsigned int GetFreeChunksCount() { return pFreeChunks; }
	bool IsChunkFree(unsigned int chunk) { return pChunks[chunk].Free; }

	MEMemoryBlock Allocate(unsigned int size);
	void Free(const MEMemoryBlock& block);

	void DumpMemory(void * dest) { memcpy(dest, pData, pTotalSize); }

	friend class MEMemoryBlock;
};


