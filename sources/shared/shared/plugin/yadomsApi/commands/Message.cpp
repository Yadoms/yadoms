#include "stdafx.h"
#include "message.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CMessage::CMessage(const std::string& command)
{
   try
   {
      shared::CDataContainer yadomsCommandTree(command);
      m_from = yadomsCommandTree.get<std::string>("to");
	  m_to = yadomsCommandTree.get<std::string>("from");
	  m_body = yadomsCommandTree.get<std::string>("body");
   }
   catch (shared::exception::CException & e)
   {
      BOOST_ASSERT_MSG(false, "Invalid message command");
      throw shared::exception::CInvalidParameter("Invalid message command \"" + command + "\" : " + e.what());
   }
}

CMessage::~CMessage()
{
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

} } } } // namespace shared::plugin::yadomsApi::commands

