#pragma once
#include "IManuallyDeviceCreationData.h"
#include "IDeviceCommand.h"


namespace shared { namespace plugin { namespace yPluginApi
{
   //-----------------------------------------------------
   ///\brief The API used for test before manually create devices
   //-----------------------------------------------------
   class IManuallyDeviceCreationTestData
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IManuallyDeviceCreationTestData() {}

      //-----------------------------------------------------
      ///\brief               Get the command
      ///\return              The command to send
      //-----------------------------------------------------
      virtual boost::shared_ptr<const IDeviceCommand> getCommand() const = 0;

      //-----------------------------------------------------
      ///\brief               Get the device parameters
      ///\return              The device parameters, as JSON string
      //-----------------------------------------------------
      virtual const std::string& getDeviceParameters() const = 0;

      //-----------------------------------------------------
      ///\brief Describe the command as string
      ///\return Human-readable string representing the command
      //-----------------------------------------------------
      virtual const std::string toString() const = 0;

};
	
} } } // namespace shared::plugin::yPluginApi	
	
	