#include "MEMemoryPool.h"

void* MEMemoryBlock::GetPointer() {
	return &(pOwner->pData[pStartingChunk * pOwner->pChunkSize]);
}

bool MEMemoryBlock::operator==(const MEMemoryBlock& other) {
	return pOwner == other.pOwner && pStartingChunk == other.pStartingChunk;
}

unsigned int MEMemoryPool::CalculateNeededChunks(unsigned int size) {
	return size / pChunkSize + (size % pChunkSize == 0 ? 0 : 1);
}

bool MEMemoryPool::FindFreeBlock(unsigned int chunks, unsigned int& startingChunk) {
	for(int i = 0; i < pNumChunks; i++) {
		if(pChunks[i].Free) {

			int found = 0;
			int cur = i;

			for(; cur < i + chunks && cur < pNumChunks; cur++) {
				if(pChunks[cur].Free) {
					found++;
				} else {
					cur = i + chunks - 1;
					break;
				}
			}

			if(found == chunks)  {
				startingChunk = i;
				return true;
			} else {
				i = cur;
				continue;
			}
		}
	}

	return false;
}

void MEMemoryPool::Reallocate(unsigned int newChunks) {
	
	unsigned int oldChunks = pNumChunks;

	pNumChunks += newChunks;
	pTotalSize = pNumChunks * pChunkSize;
	pFreeChunks += (pNumChunks - oldChunks);
	pData = (char*)realloc(pData, pTotalSize);
	pChunks = (MEChunkInfo*)realloc(pChunks, pNumChunks * sizeof(MEChunkInfo));

	for(int i = oldChunks; i < pNumChunks; i++)
		pChunks[i].Free = true;

}

MEMemoryPool::MEMemoryPool(unsigned int preferredSize, unsigned int chunkSize)
{
	pChunkSize = chunkSize;
	pFreeChunks = pNumChunks = CalculateNeededChunks(preferredSize);
	pTotalSize = pChunkSize * pNumChunks;
	pData = (char*)malloc(pTotalSize);
	pChunks = (MEChunkInfo*)malloc(sizeof(MEChunkInfo) * pNumChunks);
}

MEMemoryPool::~MEMemoryPool(void)
{
	delete[] pData;
	delete[] pChunks;
}

MEMemoryBlock MEMemoryPool::Allocate(unsigned int size) {
	unsigned int neededChunks = CalculateNeededChunks(size);
	unsigned int startingChunk;

	if(!FindFreeBlock(neededChunks, startingChunk)) {
		Reallocate(neededChunks);
		FindFreeBlock(neededChunks, startingChunk);
	}

	MEMemoryBlock result;

	result.pOwner = this;
	result.pStartingChunk = startingChunk;
	result.pChunksCount = neededChunks;

	for(int i = startingChunk; i < neededChunks + startingChunk; i++)
		pChunks[i].Free = false;

	pFreeChunks -= neededChunks;

	return result;
}

void MEMemoryPool::Free(const MEMemoryBlock& block) {
	for(int i = block.pStartingChunk; i < block.pStartingChunk + block.pChunksCount; i++)
		pChunks[i].Free = true;

	pFreeChunks += block.pChunksCount;
}