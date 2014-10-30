#pragma once

#include <shared/DataContainer.h>

namespace shared { namespace plugin { namespace yadomsApi
{
   //-----------------------------------------------------
   ///\brief The API used for manually created devices
   //-----------------------------------------------------
   class IManuallyDeviceCreationData
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IManuallyDeviceCreationData() {}

      //-----------------------------------------------------
      ///\brief               Get the device
      ///\return              The device to create
      //-----------------------------------------------------
      virtual const std::string& getDeviceName() const = 0;

      //-----------------------------------------------------
      ///\brief               Get the device configuration
      ///\return              The device parameters (in data container)
      //-----------------------------------------------------
      virtual const shared::CDataContainer & getConfiguration() const = 0;
};
	
} } } // namespace shared::plugin::yadomsApi	
	
	