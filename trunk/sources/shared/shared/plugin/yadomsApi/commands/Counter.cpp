#include "stdafx.h"
#include "Counter.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CCounter::CCounter(const std::string& keywordName, bool isIncrement)
   :m_keywordName(keywordName), m_count(0), m_measureType(isIncrement ? kIncrement : kTotalizer)
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

const std::string CCounter::formatValue(const std::string& currentValue) const
{
   return boost::lexical_cast<std::string>((m_measureType == IHistorizable::kIncrement && !currentValue.empty()) ? count() + boost::lexical_cast<unsigned int>(currentValue) : count());
}

unsigned int CCounter::count() const
{
   return m_count;
}

IHistorizable::EMeasureType CCounter::getMeasureType() const
{
   return m_measureType;
}

} } } } // namespace shared::plugin::yadomsApi::commands

