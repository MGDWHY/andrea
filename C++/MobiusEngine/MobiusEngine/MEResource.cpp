#include "MEResource.h"
#include "MEMemoryManager.h"

MEResource::MEResource(void)
{
	MEMemoryManager::AddResource(this);
}


MEResource::~MEResource(void)
{
	MEMemoryManager::RemoveResource(this);
}