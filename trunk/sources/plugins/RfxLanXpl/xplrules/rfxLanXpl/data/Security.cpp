#include "stdafx.h"
#include "Security.h"
#include <shared/plugin/yadomsApi/StandardUnits.h>

namespace xplrules { namespace rfxLanXpl { namespace data {

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

   const shared::plugin::yadomsApi::CStandardCapacity& SecurityCapacity = shared::plugin::yadomsApi::CStandardCapacity("security", shared::plugin::yadomsApi::CStandardUnits::NoUnits, shared::plugin::yadomsApi::EKeywordDataType::kNoData);

   CSecurity::CSecurity(const std::string& keywordName, const shared::plugin::yadomsApi::EKeywordAccessMode& accessMode)
      :m_keywordName(keywordName), m_accessMode(accessMode)
   {
   }

   CSecurity::~CSecurity()
   {
   }

   const std::string& CSecurity::getKeyword() const
   {
      return m_keywordName;
   }

   const shared::plugin::yadomsApi::CStandardCapacity& CSecurity::getCapacity() const
   {
      return SecurityCapacity;
   }

   const shared::plugin::yadomsApi::EKeywordAccessMode& CSecurity::getAccessMode() const
   {
      return m_accessMode;
   }

   
   void CSecurity::set(const shared::CDataContainer& yadomsCommand)
   {
      m_command = yadomsCommand.get<ECommands>("command");
      
      if(yadomsCommand.hasValue("tamper"))
         m_tamper = yadomsCommand.get<bool>("tamper");

      if(yadomsCommand.hasValue("lowBattery"))
         m_lowBattery = yadomsCommand.get<bool>("lowBattery");

      if(yadomsCommand.hasValue("delay"))
         m_delay = yadomsCommand.get<int>("delay");
   }
   
   const std::string CSecurity::formatValue() const
   {
      shared::CDataContainer yadomsCommand;
      yadomsCommand.set("command", m_command);

      if(m_tamper.isDefined())
         yadomsCommand.set("tamper", m_tamper());
         
      if(m_lowBattery.isDefined())
         yadomsCommand.set("lowBattery", m_lowBattery());

      if(m_delay.isDefined())
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

   const shared::CField<bool> & CSecurity::isTampered() const
   {
      return m_tamper;
   }

   const shared::CField<bool> &  CSecurity::isLowBattery() const
   {
      return m_lowBattery;
   }
   
   const shared::CField<int> & CSecurity::getDelay() const
   {
      return m_delay;
   }
   


   const shared::plugin::yadomsApi::historization::EMeasureType& CSecurity::getMeasureType() const
   {
      static const shared::plugin::yadomsApi::historization::EMeasureType MeasureType(shared::plugin::yadomsApi::historization::EMeasureType::kAbsolute);
      return MeasureType;
   }
















} } } // namespace xplrules::rfxLanXpl::data

