#pragma once

#include <shared/xpl/XplMessage.h>
#include "database/entities/Entities.h"

namespace communication { namespace rules {

   typedef std::string DeviceIdentifier;
   typedef std::map<std::string, std::string> MessageContent;

   class IRule
   {
   public:
      virtual ~IRule(){}

      //------------------------------------
      ///\brief Provide the device addess
      ///\param [in] msg the message from which extract the device address
      ///\return the device identifier
      //------------------------------------
      virtual const DeviceIdentifier GetDeviceAddressFromMessage(shared::xpl::CXplMessage & msg) = 0;

      //------------------------------------
      ///\brief Provide all the usefull <key, value> contained in message
      ///\param [in] msg the message from which extract the data
      ///\return the <key, value> contained in message
      //------------------------------------
      virtual MessageContent ExtractMessageData(shared::xpl::CXplMessage & msg) = 0;

      //------------------------------------
      ///\brief Identify keywords inside message
      ///\param [in] msg the message from which extract the keyword data
      ///\return the keywords data contained in message
      //------------------------------------
      virtual std::vector< boost::shared_ptr<database::entities::CKeyword> > identifyKeywords(shared::xpl::CXplMessage & msg) = 0;
   };


} //namespace rules
} //namespace communication

