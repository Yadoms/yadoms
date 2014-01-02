#pragma once

#include "IHardwareRequester.h"
#include "IConfigurationRequester.h"
#include "IDeviceRequester.h"
#include "IAcquisitionRequester.h"
#include "IKeywordRequester.h"

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

   //--------------------------------------------------------------
	/// \Brief		get the device requester
	/// \return 	the device requester
	/// \throws 		
	//--------------------------------------------------------------  
   virtual boost::shared_ptr<IDeviceRequester> getDeviceRequester()  = 0;

   //--------------------------------------------------------------
	/// \Brief		get the acquistion requester
	/// \return 	the acquistion requester
	/// \throws 		
	//--------------------------------------------------------------  
   virtual boost::shared_ptr<IAcquisitionRequester> getAcquisitionRequester()  = 0;

   //--------------------------------------------------------------
	/// \Brief		get the keyword requester
	/// \return 	the keyword requester
	/// \throws 		
	//--------------------------------------------------------------  
   virtual boost::shared_ptr<IKeywordRequester> getKeywordRequester() = 0;


protected:
	IDataProvider()
	{
	}
};
