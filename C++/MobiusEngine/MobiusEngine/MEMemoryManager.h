#pragma once

#include <vector>
#include "MEResource.h"


class MEMemoryManager
{
private:

	static std::vector<MEResource*> resources;

	MEMemoryManager(void);

	static void AddResource(MEResource * resource);
	static void RemoveResource(MEResource * resource);
public:
	
	static unsigned int GetUsedSystemMemory();

	~MEMemoryManager(void);

	friend class MEResource;
};

