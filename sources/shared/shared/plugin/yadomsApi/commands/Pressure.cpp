#include "stdafx.h"
#include "Pressure.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CPressure::CPressure(const std::string& keywordName)
   :m_keywordName(keywordName), m_pressure(0)
{
}

CPressure::~CPressure()
{
}

const std::string& CPressure::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CPressure::getCapacity() const
{
   return CStandardCapacities::Pressure;
}

void CPressure::set(const shared::CDataContainer& yadomsCommand)
{
   m_pressure = yadomsCommand.get<int>("pressure");
}

void CPressure::set(int pressure)
{
   m_pressure = pressure;
}

const std::string CPressure::formatValue() const
{
   return boost::lexical_cast<std::string>(pressure());
}

int CPressure::pressure() const
{
   return m_pressure;
}

} } } } // namespace shared::plugin::yadomsApi::commands

