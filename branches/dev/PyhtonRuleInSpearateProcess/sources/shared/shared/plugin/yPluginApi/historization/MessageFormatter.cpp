#include "stdafx.h"
#include "MessageFormatter.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>


namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

CMessageFormatter::CMessageFormatter(int from, int to, const std::string& body)
   :m_from(from), m_to(to), m_body(body)
{
}

CMessageFormatter::CMessageFormatter(const std::string& yadomsCommand)
{
   CDataContainer command(yadomsCommand);
   m_from = command.exists("from") ? command.get<int>("from") : 0;
   m_to = command.exists("to") ? command.get<int>("to") : 0;
   m_body = command.exists("body") ? command.get<std::string>("body") : std::string();
}

CMessageFormatter::~CMessageFormatter()
{
}

std::string CMessageFormatter::formatValue() const
{
   CDataContainer yadomsCommand;
   yadomsCommand.set("to", to());
   yadomsCommand.set("from", from());
   yadomsCommand.set("body", body());
   return yadomsCommand.serialize();
}

int CMessageFormatter::from() const
{
   return m_from;
}

int CMessageFormatter::to() const
{
   return m_to;
}

const std::string& CMessageFormatter::body() const
{
   return m_body;
}

} } } } // namespace shared::plugin::yPluginApi::historization

