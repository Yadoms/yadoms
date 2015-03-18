#include "stdafx.h"
#include "Message.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>
#include <shared/StringExtension.h>


namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

CMessage::CMessage(const std::string& keywordName, const std::string& associatedRecipientField, const EKeywordAccessMode& accessMode)
   :m_keywordName(keywordName), m_associatedRecipientField(associatedRecipientField), m_accessMode(accessMode)
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

void CMessage::set(const std::string& yadomsCommand)
{
   m_content.reset(new CMessageFormatter(yadomsCommand));
}

void CMessage::set(int from, int to, const std::string& body)
{
   m_content.reset(new CMessageFormatter(from, to, body));
}

std::string CMessage::formatValue() const
{
   return !m_content ? std::string() : m_content->formatValue();
}

int CMessage::from() const
{
   return !m_content ? 0 : m_content->from();
}

int CMessage::to() const
{
   return !m_content ? 0 : m_content->to();
}

const std::string& CMessage::body() const
{
   return !m_content ? CStringExtension::EmptyString : m_content->body();
}

const EMeasureType& CMessage::getMeasureType() const
{
   static const EMeasureType MeasureType(EMeasureType::kAbsolute);
   return MeasureType;
}

CDataContainer CMessage::getTypeInfo() const
{
   CDataContainer info;
   info.set("associatedRecipientField", m_associatedRecipientField);
   return info;
}

} } } } // namespace shared::plugin::yPluginApi::historization

