#include "stdafx.h"
#include "BatteryLevel.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CBatteryLevel::CBatteryLevel(const std::string& keywordName)
   :m_keywordName(keywordName), m_batteryLevel(0)
{
}

CBatteryLevel::~CBatteryLevel()
{
}

const std::string& CBatteryLevel::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CBatteryLevel::getCapacity() const
{
   return CStandardCapacities::BatteryLevel;
}

void CBatteryLevel::set(const shared::CDataContainer& yadomsCommand)
{
   m_batteryLevel = Normalize(yadomsCommand.get<int>("batteryLevel"));
}

void CBatteryLevel::set(int batteryLevel)
{
   m_batteryLevel = Normalize(batteryLevel);
}

const std::string CBatteryLevel::formatValue() const
{
   return boost::lexical_cast<std::string>(batteryLevel());
}

int CBatteryLevel::Normalize(int value)
{
   if (value > 100)
      return 100;
   if (value < 0)
      return 0;
   return value;
}

int CBatteryLevel::batteryLevel() const
{
   return m_batteryLevel;
}

} } } } // namespace shared::plugin::yadomsApi::commands

