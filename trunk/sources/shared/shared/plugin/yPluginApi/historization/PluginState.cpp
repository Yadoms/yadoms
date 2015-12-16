#include "stdafx.h"
#include "PluginState.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>
#include <shared/StringExtension.h>


namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

DECLARE_ENUM_IMPLEMENTATION(EPluginState,
   ((Unknown))
   ((Error))
   ((Stopped))
   ((Running))
   ((Custom))
);

CPluginState::CPluginState()
{
}

CPluginState::~CPluginState()
{
}

const std::string& CPluginState::getKeyword() const
{
   static const std::string keyword("state");
   return keyword;
}

const CStandardCapacity& CPluginState::getCapacity() const
{
   return CStandardCapacities::PluginState;
}

const EKeywordAccessMode& CPluginState::getAccessMode() const
{
   static const EKeywordAccessMode AccessMode(EKeywordAccessMode::kGet);
   return AccessMode;
}

void CPluginState::set(const EPluginState& state, const std::string& messageId)
{
   m_state = state;
   m_messageId = messageId;
}


EPluginState CPluginState::state() const
{
   return m_state;
}

const std::string& CPluginState::messageId() const
{
   return m_messageId;
}

std::string CPluginState::formatValue() const
{
   CDataContainer yadomsCommand;
   yadomsCommand.set("state", state());
   yadomsCommand.set("messageId", messageId());
   return yadomsCommand.serialize();
}

const EMeasureType& CPluginState::getMeasureType() const
{
   static const EMeasureType MeasureType(EMeasureType::kAbsolute);
   return MeasureType;
}

CDataContainer CPluginState::getTypeInfo() const
{
   return CDataContainer();
}

} } } } // namespace shared::plugin::yPluginApi::historization

