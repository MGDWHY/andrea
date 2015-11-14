#include "MEMemoryManager.h"

// ----- MEMemoryManager ------

std::vector<MEResource*> MEMemoryManager::resources;

MEMemoryManager::MEMemoryManager(void)
{
}


MEMemoryManager::~MEMemoryManager(void)
{
}

void MEMemoryManager::AddResource(MEResource * resource) {
	MEMemoryManager::resources.push_back(resource);
}

void MEMemoryManager::RemoveResource(MEResource * resource) {
	for(std::vector<MEResource*>::iterator it = resources.begin(); it != resources.end(); it++)
		if(*it == resource) {
			MEMemoryManager::resources.erase(it);
			return;
		}
}

unsigned int MEMemoryManager::GetUsedSystemMemory() {
	unsigned int memory = 0;
	for(std::vector<MEResource*>::iterator it = resources.begin(); it != resources.end(); it++)
		memory += (*it)->GetUsedSystemMemory();
	return memory;
}