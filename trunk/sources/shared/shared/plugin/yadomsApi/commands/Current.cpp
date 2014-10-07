#include "stdafx.h"
#include "Current.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CCurrent::CCurrent(const std::string& keywordName)
   :m_keywordName(keywordName), m_current(0.0)
{
}

CCurrent::~CCurrent()
{
}

const std::string& CCurrent::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CCurrent::getCapacity() const
{
   return CStandardCapacities::Current;
}

void CCurrent::set(const shared::CDataContainer& yadomsCommand)
{
   m_current = yadomsCommand.get<double>("current");
}

void CCurrent::set(double current)
{
   m_current = current;
}

const std::string CCurrent::formatValue(const std::string& currentValue) const
{
   return boost::lexical_cast<std::string>(current());
}

double CCurrent::current() const
{
   return m_current;
}

IHistorizable::EMeasureType CCurrent::getMeasureType() const
{
   return kAbsolute;
}

} } } } // namespace shared::plugin::yadomsApi::commands

