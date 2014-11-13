#include "stdafx.h"
#include "DateTimeBasic.h"
#include <shared/tools/Random.h>
#include <shared/exception/NotSupported.hpp>
#include "data/Log.h"
#include <shared/plugin/yadomsApi/historization/Historizers.h>

namespace xplrules { namespace rfxLanXpl {

   xplcore::CXplMessageSchemaIdentifier  CDateTimeBasic::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("datetime.basic");
   
   std::string CDateTimeBasic::m_keywordDateTime = "datetime";
   
   CDateTimeBasic::CDateTimeBasic()
   {
   }

   CDateTimeBasic::~CDateTimeBasic()
   {
   }

   // IRule implemntation
   const xplcore::CXplMessageSchemaIdentifier CDateTimeBasic::getProtocol()
   {
      return m_protocol;
   }

   const CDeviceIdentifier CDateTimeBasic::getDeviceAddressFromMessage(xplcore::CXplMessage & msg)
   {
      return CDeviceIdentifier("datetime", "DateTime message", m_protocol, m_protocol);
   }

  
   KeywordList CDateTimeBasic::identifyKeywords(xplcore::CXplMessage & msg)
   {
      KeywordList keywords;
      keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new shared::plugin::yadomsApi::historization::CDateTime(m_keywordDateTime, shared::plugin::yadomsApi::EKeywordAccessMode::kGet)));
      return keywords;
   }
   // [END] IRule implemntation


   // IReadRule implemntation
   MessageContent CDateTimeBasic::extractMessageData(xplcore::CXplMessage & msg)
   {
      MessageContent data;

      boost::shared_ptr< shared::plugin::yadomsApi::historization::CDateTime > datetime(new shared::plugin::yadomsApi::historization::CDateTime(m_keywordDateTime, shared::plugin::yadomsApi::EKeywordAccessMode::kGet));

      //get value from message
      std::string s = msg.getBodyValue(m_keywordDateTime);
      //insert caracters to allow easy parsing
      s.insert(12, ":");
      s.insert(10, ":");
      s.insert(8, " ");
      s.insert(6, "-");
      s.insert(4, "-");
      s += ".000";
      //parse date to ptime
      boost::posix_time::ptime t(boost::posix_time::time_from_string(s));
     
      //save
      datetime->set(t);
      data.push_back(datetime);
      return data;
   }
   // [END] IReadRule implemntation



} //namespace rfxLanXpl
} //namespace xplrules
