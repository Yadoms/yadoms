#include "stdafx.h"
#include "DateTimeBasic.h"
#include <shared/plugin/yPluginApi/historization/Historizers.h>

namespace xplrules
{
   namespace rfxLanXpl
   {
      xplcore::CXplMessageSchemaIdentifier CDateTimeBasic::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("datetime.basic");

      std::string CDateTimeBasic::m_keywordDateTime = "datetime";

      CDateTimeBasic::CDateTimeBasic()
      {
      }

      CDateTimeBasic::~CDateTimeBasic()
      {
      }

      // IRule implementation
      xplcore::CXplMessageSchemaIdentifier CDateTimeBasic::getProtocol()
      {
         return m_protocol;
      }

      CDeviceIdentifier CDateTimeBasic::getDeviceAddressFromMessage(xplcore::CXplMessage& msg)
      {
         return CDeviceIdentifier("datetime", "DateTime message", m_protocol, m_protocol);
      }


      KeywordList CDateTimeBasic::identifyKeywords(xplcore::CXplMessage& msg)
      {
         KeywordList keywords;
         keywords.push_back(boost::make_shared<shared::plugin::yPluginApi::historization::CDateTime>(m_keywordDateTime,
                                                                                                     shared::plugin::yPluginApi::EKeywordAccessMode::kGet));
         return keywords;
      }

      // [END] IRule implementation


      // IReadRule implementation
      MessageContent CDateTimeBasic::extractMessageData(xplcore::CXplMessage& msg)
      {
         MessageContent data;

         auto datetime(boost::make_shared<shared::plugin::yPluginApi::historization::CDateTime>(m_keywordDateTime,
                                                                                                shared::plugin::yPluginApi::EKeywordAccessMode::kGet));

         //get value from message
         auto s = msg.getBodyValue(m_keywordDateTime);
         //insert caracters to allow easy parsing
         s.insert(12, ":");
         s.insert(10, ":");
         s.insert(8, " ");
         s.insert(6, "-");
         s.insert(4, "-");
         s += ".000";
         //parse date to ptime
         auto t(boost::posix_time::time_from_string(s));

         //save
         datetime->set(t);
         data.push_back(datetime);
         return data;
      }

      // [END] IReadRule implementation
   } //namespace rfxLanXpl
} //namespace xplrules


