#include "stdafx.h"
#include "Frequency.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CFrequency::CFrequency(const std::string& keywordName)
   :m_keywordName(keywordName), m_frequency(0.0)
{
}

CFrequency::~CFrequency()
{
}

const std::string& CFrequency::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CFrequency::getCapacity() const
{
   return CStandardCapacities::Frequency;
}

void CFrequency::set(const shared::CDataContainer& yadomsCommand)
{
   m_frequency = yadomsCommand.get<double>("frequency");
}

void CFrequency::set(double frequency)
{
   m_frequency = frequency;
}

const std::string CFrequency::formatValue(const std::string& currentValue) const
{
   return boost::lexical_cast<std::string>(frequency());
}

double CFrequency::frequency() const
{
   return m_frequency;
}

IHistorizable::EMeasureType CFrequency::getMeasureType() const
{
   return kAbsolute;
}

} } } } // namespace shared::plugin::yadomsApi::commands

