#include "stdafx.h"
#include "Temperature.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CTemperature::CTemperature(const std::string& keywordName)
   :m_keywordName(keywordName), m_temperature(0.0)
{
}

CTemperature::~CTemperature()
{
}

const std::string& CTemperature::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CTemperature::getCapacity() const
{
   return CStandardCapacities::Temperature;
}

void CTemperature::set(const shared::CDataContainer& yadomsCommand)
{
   m_temperature = yadomsCommand.get<double>("temperature");
}

void CTemperature::set(double temperature)
{
   m_temperature = temperature;
}

const std::string CTemperature::formatValue(const std::string& currentValue) const
{
   return boost::lexical_cast<std::string>(temperature());
}

double CTemperature::temperature() const
{
   return m_temperature;
}

IHistorizable::EMeasureType CTemperature::getMeasureType() const
{
   return kAbsolute;
}

} } } } // namespace shared::plugin::yadomsApi::commands

