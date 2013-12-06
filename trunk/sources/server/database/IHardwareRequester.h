#pragma once

#include "entities/Hardware.h"

class IHardwareRequester
{
public:
   // IDatabaseProvider implementation
   virtual bool createHardware(boost::shared_ptr<CHardware> hardwareToRemove) = 0;

	//Read
	virtual boost::shared_ptr<CHardware> getHardware(const int hardwareId) = 0;
	virtual std::vector<boost::shared_ptr<CHardware> > getHardwares() = 0;
	
	//Update
	virtual bool updateHardware(boost::shared_ptr<CHardware> hardwareToUpdate) = 0;
   
	//Delete
   virtual bool removeHardware(boost::shared_ptr<CHardware> hardwareToRemove) = 0;

protected:
	IHardwareRequester()
	{
	}

public:
	virtual ~IHardwareRequester()
	{
	}
};
