#pragma once
#include <shared/DataContainer.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief The set device configuration command
         //-----------------------------------------------------
         class ISetDeviceConfiguration
         {
         public:
            //-----------------------------------------------------
            ///\brief               Destructor
            //-----------------------------------------------------
            virtual ~ISetDeviceConfiguration()
            {
            }

            //-----------------------------------------------------
            ///\brief               Get the device to which apply new configuration
            ///\return              The device name
            //-----------------------------------------------------
            virtual const std::string& name() const = 0;

            //-----------------------------------------------------
            ///\brief               Get the device type
            ///\return              The device type
            //-----------------------------------------------------
            virtual const std::string& type() const = 0;

            //-----------------------------------------------------
            ///\brief               Get the new configuration
            ///\return              The configuration
            //-----------------------------------------------------
            virtual const CDataContainer& configuration() const = 0;
         };
      }
   }
} // namespace shared::plugin::yPluginApi	
	
	