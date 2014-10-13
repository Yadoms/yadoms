#include "stdafx.h"
#include "Rain.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CRain::CRain(const std::string& keywordName, const EMeasureType& measureType)
   :m_keywordName(keywordName), m_rain(0.0), m_measureType(measureType)
{
}

CRain::~CRain()
{
}

const std::string& CRain::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CRain::getCapacity() const
{
   return CStandardCapacities::Rain;
}

void CRain::set(const shared::CDataContainer& yadomsCommand)
{
   m_rain = yadomsCommand.get<double>("rain");
}

void CRain::set(double rain)
{
   m_rain = rain;
}

const std::string CRain::formatValue() const
{
   return boost::lexical_cast<std::string>(rain());
}

double CRain::rain() const
{
   return m_rain;
}

const EMeasureType& CRain::getMeasureType() const
{
   return m_measureType;
}

} } } } // namespace shared::plugin::yadomsApi::historization

