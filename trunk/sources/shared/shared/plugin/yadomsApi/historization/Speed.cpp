#include "stdafx.h"
#include "Speed.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CSpeed::CSpeed(const std::string& keywordName, const EMeasureType& measureType)
   :m_keywordName(keywordName), m_speed(0.0), m_measureType(measureType)
{
}

CSpeed::~CSpeed()
{
}

const std::string& CSpeed::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CSpeed::getCapacity() const
{
   return CStandardCapacities::Rain;
}

void CSpeed::set(const shared::CDataContainer& yadomsCommand)
{
   m_speed = yadomsCommand.get<double>("speed");
}

void CSpeed::set(double speed)
{
   m_speed = speed;
}

const std::string CSpeed::formatValue() const
{
   return boost::lexical_cast<std::string>(speed());
}

double CSpeed::speed() const
{
   return m_speed;
}

const EMeasureType& CSpeed::getMeasureType() const
{
   return m_measureType;
}

} } } } // namespace shared::plugin::yadomsApi::historization

