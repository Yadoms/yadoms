#pragma once

#include <shared/DataContainer.h>

class IDeviceState
{
public:

   // ----------------------------------------
   /// \brief	        Destructor
   // ---------------------------------------- 
   virtual ~IDeviceState() = default;

   /**
    * \brief  Get Device State
    * \return Returns information about the device like name, serial number, version of the firmware, model etc.
    */
   virtual shared::CDataContainerSharedPtr getDeviceState() = 0;

   /**
    * \brief  Get Wi-Fi State
    * \return Returns Wi-Fi state.
    */
   virtual shared::CDataContainerSharedPtr getWifiState() = 0;

   /**
    * \brief  Get Bluetooth State
    * \return Returns Bluetooth state.
    */
   virtual shared::CDataContainerSharedPtr getBluetoothState() = 0;

   /**
    * \brief  Get Audio State
    * \return Returns audio state such as volume.
    */
   virtual shared::CDataContainerSharedPtr getAudioState() = 0;
};
