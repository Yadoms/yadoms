#include "stdafx.h"
#include "Power.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CPower::CPower(const std::string& keywordName)
   :m_keywordName(keywordName), m_power(0.0)
{
}

CPower::~CPower()
{
}

const std::string& CPower::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CPower::getCapacity() const
{
   return CStandardCapacities::Power;
}

void CPower::set(const shared::CDataContainer& yadomsCommand)
{
   m_power = yadomsCommand.get<double>("power");
}

void CPower::set(double power)
{
   m_power = power;
}

const std::string CPower::formatValue() const
{
   return boost::lexical_cast<std::string>(power());
}

double CPower::power() const
{
   return m_power;
}

} } } } // namespace shared::plugin::yadomsApi::commands

