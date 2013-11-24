#pragma once

#include "entities/Hardware.h"
#include "boost/scoped_ptr.hpp"
#include <list>

class IHardwareRequester
{
public:
   // IDatabaseProvider implementation
   virtual bool createHardware(boost::scoped_ptr<CHardware> hardwareToRemove) = 0;

	//Read
	virtual boost::scoped_ptr<CHardware> getHardware(const int hardwareId) = 0;
	virtual std::list<boost::scoped_ptr<CHardware>> getHardwares() = 0;
	
	//Update
	virtual bool updateHardware(boost::scoped_ptr<CHardware> hardwareToUpdate) = 0;
   
	//Delete
   virtual bool removeHardware(boost::scoped_ptr<CHardware> hardwareToRemove) = 0;

protected:
	IHardwareRequester()
	{
	}

	virtual ~IHardwareRequester()
	{
	}
};
