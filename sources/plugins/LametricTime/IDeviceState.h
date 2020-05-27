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
   virtual boost::shared_ptr<shared::CDataContainer> getDeviceState() = 0;

   /**
    * \brief  Get Wi-Fi State
    * \return Returns Wi-Fi state.
    */
   virtual boost::shared_ptr<shared::CDataContainer> getWifiState() = 0;

   /**
    * \brief  Get Bluetooth State
    * \return Returns Bluetooth state.
    */
   virtual boost::shared_ptr<shared::CDataContainer> getBluetoothState() = 0;

   /**
    * \brief  Get Audio State
    * \return Returns audio state such as volume.
    */
   virtual boost::shared_ptr<shared::CDataContainer> getAudioState() = 0;
};
