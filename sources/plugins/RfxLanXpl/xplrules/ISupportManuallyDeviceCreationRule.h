#pragma once

#include "DeviceIdentifier.h"
#include "DeviceContainer.h"
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationData.h>

namespace xplrules {


   //------------------------------------
   ///\brief   Interface that identifies rules with device creation ability
   //------------------------------------
   class ISupportManuallyDeviceCreationRule
   {
   public:
      //------------------------------------
      ///\brief Destructor
      //------------------------------------
      virtual ~ISupportManuallyDeviceCreationRule(){}

      //------------------------------------
      ///\brief Provide the device and keywords
      ///\param [in] configuration  The configuration of device
      ///\return the device container (device identifier and keywords)
      //------------------------------------
      virtual const CDeviceContainer generateDeviceParameters(shared::CDataContainer & configuration) const = 0;
   };


} //namespace xplrules


