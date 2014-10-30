#include "stdafx.h"
#include "Event.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/StringExtension.h>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CEvent::CEvent(const std::string& keywordName, const EKeywordAccessMode& accessMode)
   :m_keywordName(keywordName), m_accessMode (accessMode)
{
}

CEvent::~CEvent()
{
}

const std::string& CEvent::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CEvent::getCapacity() const
{
   return CStandardCapacities::Event;
}

const EKeywordAccessMode& CEvent::getAccessMode() const
{
   return m_accessMode;
}

const std::string CEvent::formatValue() const
{
   // No value
   return shared::CStringExtension::EmptyString;
}

const EMeasureType& CEvent::getMeasureType() const
{
   static const EMeasureType MeasureType(EMeasureType::kAbsolute);
   return MeasureType;
}

} } } } // namespace shared::plugin::yadomsApi::historization

