#include "stdafx.h"
#include "Message.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

   CMessage::CMessage(const shared::CDataContainer& command)
{
   try
   {
      m_from = command.get<std::string>("to");
      m_to = command.get<std::string>("from");
      m_body = command.get<std::string>("body");
   }
   catch (shared::exception::CException & e)
   {
      BOOST_ASSERT_MSG(false, "Invalid message command");
      throw shared::exception::CInvalidParameter("Invalid message command \"" + command.serialize() + "\" : " + e.what());
   }
}

CMessage::~CMessage()
{
}

const CField<std::string>& CMessage::from() const
{
   return m_from;
}

const CField<std::string>& CMessage::to() const
{
   return m_to;
}

const CField<std::string>& CMessage::body() const
{
   return m_body;
}

} } } } // namespace shared::plugin::yadomsApi::commands

