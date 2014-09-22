#include "stdafx.h"
#include "Switch.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CSwitch::CSwitch(const std::string& keywordName)
   :m_keywordName(keywordName), m_switchLevel(0)
{
}

CSwitch::~CSwitch()
{
}

const std::string& CSwitch::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CSwitch::getCapacity() const
{
   return CStandardCapacities::Switch;
}

void CSwitch::set(const shared::CDataContainer& yadomsCommand)
{
   m_switchLevel = NormalizeLevel(yadomsCommand.get<int>("level"));
}

void CSwitch::set(bool isOn)
{
   m_switchLevel = isOn ? 100 : 0;
}

void CSwitch::set(int switchLevel)
{
   m_switchLevel = NormalizeLevel(switchLevel);
}

const std::string CSwitch::formatValue() const
{
   return boost::lexical_cast<std::string>(switchLevel());
}

int CSwitch::NormalizeLevel(int level)
{
   if (level > 100)
      return 100;
   if (level < 0)
      return 0;
   return level;
}

int CSwitch::switchLevel() const
{
   return m_switchLevel;
}

bool CSwitch::isOn() const
{
   return (m_switchLevel >= 50) ? true : false;
}

} } } } // namespace shared::plugin::yadomsApi::commands

