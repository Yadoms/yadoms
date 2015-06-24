#pragma once
#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>

namespace shared { namespace plugin { namespace yPluginApi
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
      ///\return              The command body, as string
      //-----------------------------------------------------
      virtual const std::string & getBody() const = 0;

      //-----------------------------------------------------
      ///\brief Describe the command as string
      ///\return Human-readable string representing the command
      //-----------------------------------------------------
      virtual const std::string toString() const = 0;

      //-----------------------------------------------------
      ///\brief               Get the historizable object
      ///\return              The historizable object associated with this command
      //-----------------------------------------------------
      virtual const historization::IHistorizable& getHistorizableObject() const = 0;

};
	
} } } // namespace shared::plugin::yPluginApi	
	
	