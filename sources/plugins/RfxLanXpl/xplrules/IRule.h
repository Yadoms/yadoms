#pragma once

#include "../xplcore/XplMessage.h"
#include "DeviceIdentifier.h"
#include "DeviceKeyword.h"

namespace xplrules {

   //------------------------------------
   ///\brief   A rule is an interface between devices and yadoms.
   ///         A device send XplMessage to yadoms, then the Rule read it and parse data
   ///         A widget from GUI wish to send a command to the device, then ask the Rule to prepare the message to send
   ///
   /// The aim is to parse and create XplMessage used to communicate with device. 
   /// In fact, some devices may need to have specific writing rules (for example, with RfxLanXpl firmware, XplMessages need to fill some fields depending on the protocol)
   //------------------------------------
   class IRule
   {
   public:
      //------------------------------------
      ///\brief Destructor
      //------------------------------------
      virtual ~IRule(){}

      //------------------------------------
      ///\brief Provide the device addess
      ///\param [in] msg the message from which extract the device address
      ///\return the device identifier
      //------------------------------------
      virtual const CDeviceIdentifier getDeviceAddressFromMessage(xplcore::CXplMessage & msg) = 0;

      //------------------------------------
      ///\brief Identify keywords inside message
      ///\param [in] msg the message from which extract the keyword data
      ///\return the keywords data contained in message
      //------------------------------------
      virtual std::vector< boost::shared_ptr<CDeviceKeyword> > identifyKeywords(xplcore::CXplMessage & msg) = 0;
   };


} //namespace xplrules


