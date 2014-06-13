#include "stdafx.h"
#include "message.h"
#include "../StandardValues.h"
#include <shared/serialization/PTreeToJsonSerializer.h>
#include <shared/exception/InvalidParameter.hpp>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CMessage::CMessage(const std::string& command)
{
   shared::serialization::CPtreeToJsonSerializer serializer;
   try
   {
      boost::property_tree::ptree yadomsCommandTree = serializer.deserialize(command);
      m_from = yadomsCommandTree.get<std::string>("to");
      m_to = yadomsCommandTree.get<std::string>("from");
      m_body = yadomsCommandTree.get<std::string>("body");
   }
   catch (boost::property_tree::ptree_bad_path& e)
   {
      BOOST_ASSERT_MSG(false, "Invalid message command");
      throw shared::exception::CInvalidParameter("Invalid message command \"" + command + "\" : " + e.what());
   }
   catch (boost::property_tree::ptree_bad_data& e)
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

