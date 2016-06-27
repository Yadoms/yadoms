#pragma once

#include "../xplcore/XplMessage.h"

namespace xplrules
{
   //------------------------------------
   ///\brief   Type for message content (list of historizable data)
   //------------------------------------
   typedef std::vector<boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > MessageContent;

   //------------------------------------
   ///\brief   Interface for rules which allow reading data from xpl
   //------------------------------------
   class IReadRule
   {
   public:
      //------------------------------------
      ///\brief Destructor
      //------------------------------------
      virtual ~IReadRule()
      {
      }

      //------------------------------------
      ///\brief Provide all the usefull <key, value> contained in message
      ///\param [in] msg the message from which extract the data
      ///\return the <key, value> contained in message
      //------------------------------------
      virtual MessageContent extractMessageData(xplcore::CXplMessage& msg) = 0;
   };
} //namespace xplrules


