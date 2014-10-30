#pragma once

#include "DeviceIdentifier.h"
#include "DeviceContainer.h"
#include <shared/plugin/yadomsApi/IManuallyDeviceCreationData.h>

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
      virtual const CDeviceContainer generateDeviceParameters(boost::shared_ptr<yApi::IManuallyDeviceCreationData> configuration) = 0;
   };


} //namespace xplrules


