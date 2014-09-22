#include "stdafx.h"
#include "Counter.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CCounter::CCounter(const std::string& keywordName)
   :m_keywordName(keywordName), m_count(0)
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

} } } } // namespace shared::plugin::yadomsApi::commands

