#pragma once

#include "entities/Configuration.h"

class IConfigurationRequester
{
public:
   // IDatabaseProvider implementation
   virtual bool create(boost::shared_ptr<CConfiguration> configurationToCreate) = 0;

	//Read
	virtual boost::shared_ptr<CConfiguration> getConfiguration(const std::string & section, const std::string & name) = 0;
	
	//Update
	virtual bool updateConfiguration(boost::shared_ptr<CConfiguration> configurationToUpdate) = 0;
   
	//Delete
   virtual bool removeConfiguration(boost::shared_ptr<CConfiguration> configurationToRemove) = 0;


	virtual ~IConfigurationRequester()
	{
	}
};
