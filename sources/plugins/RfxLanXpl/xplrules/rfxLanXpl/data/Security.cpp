#include "stdafx.h"
#include "Security.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/DataContainer.h>

namespace xplrules
{
   namespace rfxLanXpl
   {
      namespace data
      {
         DECLARE_ENUM_IMPLEMENTATION_NESTED(CSecurity::ECommands, ECommands,
            ((Alert))
            ((Normal))
            ((Motion))
            ((Light))
            ((Dark))
            ((ArmHome))
            ((ArmAway))
            ((Disarm))
            ((Panic))
            ((LightsOn))
            ((LightsOff))
         )

         DECLARE_CAPACITY(SecurityCapacity, "security", shared::plugin::yPluginApi::CStandardUnits::NoUnit(), shared::plugin::yPluginApi::EKeywordDataType::kNoData);

         CSecurity::CSecurity(const std::string& keywordName,
                              const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode)
            : m_keywordName(keywordName),
              m_accessMode(accessMode),
              m_command(CSecurity::ECommands::kAlert)
         {
         }

         CSecurity::~CSecurity()
         {
         }

         const std::string& CSecurity::getKeyword() const
         {
            return m_keywordName;
         }

         const shared::plugin::yPluginApi::CStandardCapacity& CSecurity::getCapacity() const
         {
            return SecurityCapacity();
         }

         const shared::plugin::yPluginApi::EKeywordAccessMode& CSecurity::getAccessMode() const
         {
            return m_accessMode;
         }


         void CSecurity::set(const std::string& yadomsCommand)
         {
            shared::CDataContainer command(yadomsCommand);
            m_command = command.get<ECommands>("command");

            if (command.exists("tamper"))
               m_tamper = command.get<bool>("tamper");

            if (command.exists("lowBattery"))
               m_lowBattery = command.get<bool>("lowBattery");

            if (command.exists("delay"))
               m_delay = command.get<int>("delay");
         }

         std::string CSecurity::formatValue() const
         {
            shared::CDataContainer yadomsCommand;
            yadomsCommand.set("command", m_command);

            if (m_tamper.isDefined())
               yadomsCommand.set("tamper", m_tamper());

            if (m_lowBattery.isDefined())
               yadomsCommand.set("lowBattery", m_lowBattery());

            if (m_delay.isDefined())
               yadomsCommand.set("delay", m_delay());

            return yadomsCommand.serialize();
         }

         void CSecurity::setCommand(const std::string& command)
         {
            m_command = command;
         }

         void CSecurity::setTampered(const bool tampered)
         {
            m_tamper = tampered;
         }

         void CSecurity::setLowBattery(const bool lowBattery)
         {
            m_lowBattery = lowBattery;
         }

         void CSecurity::setDelay(const int delay)
         {
            m_delay = delay;
         }


         const CSecurity::ECommands& CSecurity::getCommand() const
         {
            return m_command;
         }

         const shared::CField<bool>& CSecurity::isTampered() const
         {
            return m_tamper;
         }

         const shared::CField<bool>& CSecurity::isLowBattery() const
         {
            return m_lowBattery;
         }

         const shared::CField<int>& CSecurity::getDelay() const
         {
            return m_delay;
         }

         const shared::plugin::yPluginApi::EMeasureType& CSecurity::getMeasureType() const
         {
            static const auto MeasureType(shared::plugin::yPluginApi::EMeasureType::kAbsolute);
            return MeasureType;
         }

         shared::CDataContainerSharedPtr CSecurity::getTypeInfo() const
         {
            return new_CDataContainerSharedPtr();
         }

         const shared::plugin::yPluginApi::EHistoryDepth& CSecurity::getHistoryDepth() const
         {
            static const auto DefaultHistoryDepth(shared::plugin::yPluginApi::EHistoryDepth::kDefault);
            return DefaultHistoryDepth;
         }
      }
   }
} // namespace xplrules::rfxLanXpl::data


