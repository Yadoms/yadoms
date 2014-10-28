#include "stdafx.h"
#include "Message.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>


namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CMessage::CMessage(const std::string& keywordName, const EKeywordAccessMode& accessMode)
   :m_keywordName(keywordName), m_accessMode(accessMode)
{
}

CMessage::~CMessage()
{
}

const std::string& CMessage::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CMessage::getCapacity() const
{
   return CStandardCapacities::Message;
}

const EKeywordAccessMode& CMessage::getAccessMode() const
{
   return m_accessMode;
}

void CMessage::set(const shared::CDataContainer& yadomsCommand)
{
   m_from = yadomsCommand.get<std::string>("to");
   m_to = yadomsCommand.get<std::string>("from");
   m_body = yadomsCommand.get<std::string>("body");
}

void CMessage::set(const std::string& from, const std::string& to, const std::string& body)
{
   m_from = from;
   m_to = to;
   m_body = body;
}

const std::string CMessage::formatValue() const
{
   shared::CDataContainer yadomsCommand;
   yadomsCommand.set("to", to());
   yadomsCommand.set("from", from());
   yadomsCommand.set("body", body());
   return yadomsCommand.serialize();
}

const std::string& CMessage::from() const
{
   return m_from;
}

const std::string& CMessage::to() const
{
   return m_to;
}

const std::string& CMessage::body() const
{
   return m_body;
}

const EMeasureType& CMessage::getMeasureType() const
{
   static const EMeasureType MeasureType(EMeasureType::kAbsolute);
   return MeasureType;
}

} } } } // namespace shared::plugin::yadomsApi::historization

