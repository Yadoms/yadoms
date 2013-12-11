#pragma once

#include "IHardwareRequester.h"
#include "IConfigurationRequester.h"

class IDataProvider
{
public:
	virtual ~IDataProvider()
	{
	}

	//--------------------------------------------------------------
	/// \Brief		Load the database system
	/// \return 	true if database is loaded with success
	/// \throws 		
	//--------------------------------------------------------------
	virtual bool load() = 0;

	//--------------------------------------------------------------
	/// \Brief	Unoad the database system
	//--------------------------------------------------------------
	virtual void unload() = 0;

	//--------------------------------------------------------------
	/// \Brief		get the hardware requester
	/// \return 	the hardware requester
	/// \throws 		
	//--------------------------------------------------------------  
   virtual boost::shared_ptr<IHardwareRequester> getHardwareRequester() = 0;

   //--------------------------------------------------------------
	/// \Brief		get the configuration requester
	/// \return 	the configuration requester
	/// \throws 		
	//--------------------------------------------------------------  
   virtual boost::shared_ptr<IConfigurationRequester> getConfigurationRequester() = 0;


protected:
	IDataProvider()
	{
	}
};
