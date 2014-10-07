#include "stdafx.h"
#include "Energy.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CEnergy::CEnergy(const std::string& keywordName, bool isIncrement)
   :m_keywordName(keywordName), m_energy(0.0), m_measureType(isIncrement ? kIncrement : kTotalizer)
{
}

CEnergy::~CEnergy()
{
}

const std::string& CEnergy::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CEnergy::getCapacity() const
{
   return CStandardCapacities::Energy;
}

void CEnergy::set(const shared::CDataContainer& yadomsCommand)
{
   m_energy = yadomsCommand.get<double>("energy");
}

void CEnergy::set(double energy)
{
   m_energy = energy;
}

const std::string CEnergy::formatValue(const std::string& currentValue) const
{
   return boost::lexical_cast<std::string>((m_measureType == IHistorizable::kIncrement && !currentValue.empty()) ? energy() + boost::lexical_cast<double>(currentValue) : energy());
}

double CEnergy::energy() const
{
   return m_energy;
}

IHistorizable::EMeasureType CEnergy::getMeasureType() const
{
   return m_measureType;
}

} } } } // namespace shared::plugin::yadomsApi::commands

