#pragma once

#include <shared/DataContainer.h>

class ILametricDeviceState
{
public:

   // ----------------------------------------
   /// \brief	        Destructor
   // ---------------------------------------- 
   virtual ~ILametricDeviceState() = default;

   /**
    * \brief  Get Device State
    * \return Returns information about the device like name, serial number, version of the firmware, model etc.
    */
   virtual shared::CDataContainer getDeviceState() = 0;

   /**
    * \brief  Get Wi-Fi State
    * \return Returns Wi-Fi state.
    */
   virtual shared::CDataContainer getWifiState() = 0;

   /**
    * \brief  Get Bluetooth State
    * \return Returns Bluetooth state.
    */
   virtual shared::CDataContainer getBluetoothState() = 0;

   /**
    * \brief  Get Audio State
    * \return Returns audio state such as volume.
    */
   virtual shared::CDataContainer getAudioState() = 0;
};
