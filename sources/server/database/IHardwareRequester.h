#pragma once

#include "entities/Hardware.h"
#include <vector>

class IHardwareRequester
{
public:
   // IDatabaseProvider implementation
   virtual bool createHardware(CHardware & hardwareToRemove) = 0;

	//Read
	virtual CHardware getHardware(const int hardwareId) = 0;
	virtual std::vector<CHardware> getHardwares() = 0;
	
	//Update
	virtual bool updateHardware(CHardware & hardwareToUpdate) = 0;
   
	//Delete
   virtual bool removeHardware(CHardware & hardwareToRemove) = 0;

protected:
	IHardwareRequester()
	{
	}

	virtual ~IHardwareRequester()
	{
	}
};
