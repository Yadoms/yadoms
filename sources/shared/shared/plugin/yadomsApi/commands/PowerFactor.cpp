#include "stdafx.h"
#include "PowerFactor.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CPowerFactor::CPowerFactor(const std::string& keywordName)
   :m_keywordName(keywordName), m_powerFactor(0.0)
{
}

CPowerFactor::~CPowerFactor()
{
}

const std::string& CPowerFactor::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CPowerFactor::getCapacity() const
{
   return CStandardCapacities::Frequency;
}

void CPowerFactor::set(const shared::CDataContainer& yadomsCommand)
{
   m_powerFactor = Normalize(yadomsCommand.get<double>("powerFactor"));
}

void CPowerFactor::set(double powerFactor)
{
   m_powerFactor = powerFactor;
}

const std::string CPowerFactor::formatValue(const std::string& currentValue) const
{
   return boost::lexical_cast<std::string>(powerFactor());
}

double CPowerFactor::Normalize(double value)
{
   if (value > 1.0)
      return 1.0;
   if (value < -1.0)
      return -1.0;
   return value;
}

double CPowerFactor::powerFactor() const
{
   return m_powerFactor;
}

IHistorizable::EMeasureType CPowerFactor::getMeasureType() const
{
   return kAbsolute;
}

} } } } // namespace shared::plugin::yadomsApi::commands

