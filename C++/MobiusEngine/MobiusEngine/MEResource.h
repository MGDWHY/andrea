#pragma once

class MEResource
{
public:
	MEResource(void);
	~MEResource(void);

	virtual unsigned int GetUsedSystemMemory() = 0; 
};