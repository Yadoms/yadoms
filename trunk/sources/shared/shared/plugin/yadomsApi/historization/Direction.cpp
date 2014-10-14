#include "stdafx.h"
#include "Direction.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CDirection::CDirection(const std::string& keywordName, const EMeasureType& measureType)
   :m_keywordName(keywordName), m_direction(0), m_measureType(measureType)
{
}

CDirection::~CDirection()
{
}

const std::string& CDirection::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CDirection::getCapacity() const
{
   return CStandardCapacities::Direction;
}

void CDirection::set(const shared::CDataContainer& yadomsCommand)
{
   m_direction = yadomsCommand.get<int>("direction");
}

void CDirection::set(int direction)
{
   m_direction = direction;
}

const std::string CDirection::formatValue() const
{
   return boost::lexical_cast<std::string>(direction());
}

int CDirection::direction() const
{
   return m_direction;
}

const EMeasureType& CDirection::getMeasureType() const
{
   return m_measureType;
}

} } } } // namespace shared::plugin::yadomsApi::historization

