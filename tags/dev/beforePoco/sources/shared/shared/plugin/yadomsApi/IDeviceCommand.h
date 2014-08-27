#pragma once
#include <shared/DataContainer.h>

namespace shared { namespace plugin { namespace yadomsApi
{
   //-----------------------------------------------------
   ///\brief The API used for the commands on device
   //-----------------------------------------------------
   class IDeviceCommand
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IDeviceCommand() {}

      //-----------------------------------------------------
      ///\brief               Get the target device
      ///\return              The device to command
      //-----------------------------------------------------
      virtual const std::string& getTargetDevice() const = 0;

      //-----------------------------------------------------
      ///\brief               Get the target keyword
      ///\return              The keyword to command
      //-----------------------------------------------------
      virtual const std::string& getKeyword() const = 0;

      //-----------------------------------------------------
      ///\brief               Get the command body
      ///\return              The command body, as JSON string
      //-----------------------------------------------------
      virtual const shared::CDataContainer & getBody() const = 0;

      //-----------------------------------------------------
      ///\brief Describe the command as string
      ///\return Human-readable string representing the command
      //-----------------------------------------------------
      virtual const std::string toString() const = 0;

};
	
} } } // namespace shared::plugin::yadomsApi	
	
	