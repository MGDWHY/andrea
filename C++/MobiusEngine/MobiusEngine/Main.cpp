#include <iostream>
#include <fstream>
#include <ctime>
#include <Windows.h>
#include "MEMemoryPool.h"

using namespace std;

MEMemoryPool pool(10000, 128);

clock_t GetTime() {
	return (clock_t)(clock() / (double)CLOCKS_PER_SEC * 1000);
}

void PoolStat(MEMemoryPool &p) {
	cout << "Size: " << p.GetTotalSize() << " bytes" << endl;
	cout << "Chunk Size: " << p.GetChunkSize() << " bytes" << endl;
	cout << "Chunks count: " << p.GetChunksCount() << endl;
	cout << "Free chunks count: " << p.GetFreeChunksCount() << endl;
	//cout << "Chunks Status: ";
	/*for(int i = 0; i < p.GetChunksCount(); i++)
		cout << (p.IsChunkFree(i) ? '0' : '1');*/
	//cout << endl;
	cout << "--------------------------" << endl;
}

void PoolDump(MEMemoryPool &p) {
	ofstream os;
	static int dumpNum = 0;
	static char fName[20];
	char * dump = new char[p.GetTotalSize()];
	
	sprintf(fName, "dump%d.bin", dumpNum++);

	p.DumpMemory(dump);
	os.open(string(fName), ios_base::binary);
	os.write(dump, p.GetTotalSize());
	
	os.close();
	
	cout << "Memory dumped: " << fName << endl;
	cout << "--------------------------" << endl;
	delete dump;
}

MEMemoryBlock Allocate(unsigned int size) {
	MEMemoryBlock res = pool.Allocate(size);
	cout << "Allocating " << size << " (" << (res.GetChunksCount() * pool.GetChunkSize()) << ")" << " bytes" << endl;
	cout << "StartingChunk->" << res.GetStartingChunk() << " ChunksCount->" << res.GetChunksCount() << endl;
	cout << "--------------------------" << endl;
	return res;
}

void Free(MEMemoryBlock b) {
	pool.Free(b);
	cout << "Freeing " << (b.GetChunksCount() * pool.GetChunkSize()) << " bytes-block" << endl;
	cout << "--------------------------" << endl;
}

void DoStuff(int * data, unsigned int length) {
	ZeroMemory(data, length);
/*	for(int i = 0; i < length; i++)
		data[i] = rand();*/
}

int main() {
	int x;

	MEMemoryBlock blk1, blk2, blk3, blk4, blk5, blk6;

	try {

		PoolStat(pool);

		blk1 = Allocate(360);

		PoolStat(pool);
		PoolDump(pool);

		blk2 = Allocate(640);

		PoolStat(pool);
		PoolDump(pool);

		Free(blk1);

		PoolStat(pool);
		PoolDump(pool);

		blk3 = Allocate(500);

		PoolStat(pool);
		PoolDump(pool);

		blk4 = Allocate(1000);

		PoolStat(pool);
		PoolDump(pool);

		blk5 = Allocate(2000);

		PoolStat(pool);
		PoolDump(pool);

		Free(blk3);
		Free(blk4);
		PoolStat(pool);
		PoolDump(pool);

		blk4 = Allocate(700);
		PoolStat(pool);
		PoolDump(pool);


	}
	catch(char * ex) {
		cout << "Exception: " << ex << endl;
	}

	unsigned int arraySize = 100;
	int numAlloc = 10000;
	clock_t startTime;

	cout << "Array Test (Pool)" << endl << "-------------------" << endl;
	startTime = GetTime();
	for(int i = 0; i < numAlloc; i++) {
		blk1 = pool.Allocate(arraySize * sizeof(int));
		ZeroMemory(blk1.GetPointer(), arraySize * sizeof(int));
		pool.Free(blk1);
	}
	cout << "Time: " << (GetTime() - startTime) << " ms" << endl << "-----------------" << endl;

	cout << "Array Test (Heap)" << endl << "-------------------" << endl;
	startTime = GetTime();
	for(int i = 0; i < numAlloc; i++) {
		char *x = (char*)malloc(arraySize * sizeof(int));
		ZeroMemory(blk1.GetPointer(), arraySize * sizeof(int));
		free(x);
	}
	cout << "Time: " << (GetTime() - startTime) << " ms " << endl << "-----------------" << endl;


	cin >> x;
}