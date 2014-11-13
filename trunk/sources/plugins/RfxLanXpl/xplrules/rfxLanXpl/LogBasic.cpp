#include "stdafx.h"
#include "LogBasic.h"
#include <shared/tools/Random.h>
#include <shared/exception/NotSupported.hpp>
#include "data/Log.h"

namespace xplrules { namespace rfxLanXpl {

   xplcore::CXplMessageSchemaIdentifier  CLogBasic::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("log.basic");
   std::string CLogBasic::m_keywordType = "type";
   std::string CLogBasic::m_keywordText = "text";
   std::string CLogBasic::m_keywordCode = "code";
   
   
   CLogBasic::CLogBasic()
   {
   }

   CLogBasic::~CLogBasic()
   {
   }

   // IRule implemntation
   const xplcore::CXplMessageSchemaIdentifier CLogBasic::getProtocol()
   {
      return m_protocol;
   }

   const CDeviceIdentifier CLogBasic::getDeviceAddressFromMessage(xplcore::CXplMessage & msg)
   {
      return CDeviceIdentifier("log", "Log message", m_protocol, m_protocol);
   }

  
   KeywordList CLogBasic::identifyKeywords(xplcore::CXplMessage & msg)
   {
      KeywordList keywords;
      keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new data::CLog("log", shared::plugin::yadomsApi::EKeywordAccessMode::kGet)));
      return keywords;
   }
   // [END] IRule implemntation


   // IReadRule implemntation
   MessageContent CLogBasic::extractMessageData(xplcore::CXplMessage & msg)
   {
      MessageContent data;

      boost::shared_ptr< data::CLog > log(new data::CLog("log", shared::plugin::yadomsApi::EKeywordAccessMode::kGet));
      
      std::string code;
      if (msg.getBody().find(m_keywordCode) != msg.getBody().end())
         code = msg.getBodyValue(m_keywordCode);
      
      log->set(msg.getBodyValue(m_keywordType), msg.getBodyValue(m_keywordText), code);

      data.push_back(log);
      return data;
   }
   // [END] IReadRule implemntation



} //namespace rfxLanXpl
} //namespace xplrules
