#include "stdafx.h"
#include "Log.h"
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

   DECLARE_ENUM_IMPLEMENTATION_NESTED(CLog::ELogLevel, ELogLevel,
      ((Inf))
      ((Wrn))
      ((Err))
   );

   const shared::plugin::yadomsApi::CStandardCapacity& LogCapacity = shared::plugin::yadomsApi::CStandardCapacity("Log", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordDataType::kString);


   CLog::CLog(const std::string& keywordName, const shared::plugin::yadomsApi::EKeywordAccessMode& accessMode)
      :m_keywordName(keywordName), m_accessMode(accessMode)
   {
   }

   CLog::~CLog()
   {
   }

   const std::string& CLog::getKeyword() const
   {
      return m_keywordName;
   }

   const shared::plugin::yadomsApi::CStandardCapacity& CLog::getCapacity() const
   {
      return LogCapacity;
   }

   const shared::plugin::yadomsApi::EKeywordAccessMode& CLog::getAccessMode() const
   {
      return m_accessMode;
   }

   void CLog::set(const shared::CDataContainer& yadomsCommand)
   {
      m_level = yadomsCommand.get<ELogLevel>("level");
      m_text = yadomsCommand.get<std::string>("text");
      m_code = yadomsCommand.get<std::string>("code");
   }

   void CLog::set(const std::string& level, const std::string& text, const std::string& code)
   {
      m_level = level;
      m_text = text;
      m_code = code;
   }

   const std::string CLog::formatValue() const
   {
      shared::CDataContainer yadomsCommand;
      yadomsCommand.set("level", getLevel());
      yadomsCommand.set("text", getText());
      yadomsCommand.set("code", getCode());
      return yadomsCommand.serialize();
   }

   const CLog::ELogLevel& CLog::getLevel() const
   {
      return m_level;
   }

   const std::string& CLog::getText() const
   {
      return m_text;
   }

   const std::string& CLog::getCode() const
   {
      return m_code;
   }


   const shared::plugin::yadomsApi::historization::EMeasureType& CLog::getMeasureType() const
   {
      static const shared::plugin::yadomsApi::historization::EMeasureType MeasureType(shared::plugin::yadomsApi::historization::EMeasureType::kAbsolute);
      return MeasureType;
   }


















} } } // namespace xplrules::rfxLanXpl::data

