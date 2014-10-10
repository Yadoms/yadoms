#include "stdafx.h"
#include "Counter.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CCounter::CCounter(const std::string& keywordName, const EMeasureType& measureType)
   :m_keywordName(keywordName), m_count(0), m_measureType(measureType)
{
}

CCounter::~CCounter()
{
}

const std::string& CCounter::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CCounter::getCapacity() const
{
   return CStandardCapacities::Counter;
}

void CCounter::set(const shared::CDataContainer& yadomsCommand)
{
   m_count = yadomsCommand.get<unsigned int>("count");
}

void CCounter::set(unsigned int count)
{
   m_count = count;
}

const std::string CCounter::formatValue() const
{
   return boost::lexical_cast<std::string>(count());
}

unsigned int CCounter::count() const
{
   return m_count;
}

const EMeasureType& CCounter::getMeasureType() const
{
   return m_measureType;
}

} } } } // namespace shared::plugin::yadomsApi::historization

