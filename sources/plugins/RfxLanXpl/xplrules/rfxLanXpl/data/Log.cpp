#include "stdafx.h"
#include "Log.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
      {
         DECLARE_ENUM_IMPLEMENTATION_NESTED(CLog::ELogLevel, ELogLevel,
            ((Inf))
            ((Wrn))
            ((Err))
         );

         DECLARE_CAPACITY(LogCapacity, "Log", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kString);


         CLog::CLog(const std::string& keywordName,
                    const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode)
            : m_keywordName(keywordName),
              m_accessMode(accessMode),
              m_level(ELogLevel::kInf)
         {
         }

         CLog::~CLog()
         {
         }

         const std::string& CLog::getKeyword() const
         {
            return m_keywordName;
         }

         const shared::plugin::yPluginApi::CStandardCapacity& CLog::getCapacity() const
         {
            return LogCapacity();
         }

         const shared::plugin::yPluginApi::EKeywordAccessMode& CLog::getAccessMode() const
         {
            return m_accessMode;
         }

         void CLog::set(const shared::CDataContainer& yadomsCommand)
         {
            m_level = yadomsCommand.get<ELogLevel>("level");
            m_text = yadomsCommand.get<std::string>("text");
            m_code = yadomsCommand.get<std::string>("code");
         }

         void CLog::set(const std::string& level,
                        const std::string& text,
                        const std::string& code)
         {
            m_level = level;
            m_text = text;
            m_code = code;
         }

         std::string CLog::formatValue() const
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


         const shared::plugin::yPluginApi::historization::EMeasureType& CLog::getMeasureType() const
         {
            static const auto MeasureType(shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute);
            return MeasureType;
         }

         shared::CDataContainer CLog::getTypeInfo() const
         {
            return shared::CDataContainer();
         }
      }
   }
} // namespace xplrules::rfxLanXpl::data


