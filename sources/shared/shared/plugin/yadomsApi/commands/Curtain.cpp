#include "stdafx.h"
#include "Curtain.h"
#include "../StandardValues.h"
#include <shared/DataContainer.h>
#include <shared/exception/InvalidParameter.hpp>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CCurtain::CCurtain(const std::string& command)
   :m_value(kStop)
{
   try
   {
      shared::CDataContainer yadomsCommandTree(command);
      std::string cmdValue = yadomsCommandTree.get<std::string>("cmd");

      if (cmdValue == CStandardValues::Open)
         m_value = kOpen;
      else if (cmdValue == CStandardValues::Close)
         m_value = kClose;
      else if (cmdValue == CStandardValues::Stop)
         m_value = kStop;
      else
      {
         BOOST_ASSERT_MSG(false, "Wrong curtain command value");
         throw shared::exception::CInvalidParameter("Invalid curtain command \"" + command + "\" : value out of range");
      }
   }
   catch (shared::exception::CException & e)
   {
      BOOST_ASSERT_MSG(false, "Invalid curtain command");
      throw shared::exception::CInvalidParameter("Invalid curtain command \"" + command + "\" : " + e.what());
   }
}

CCurtain::~CCurtain()
{
}

CCurtain::CCurtain(ECommand command)
   :m_value(command)
{
}

CCurtain::ECommand CCurtain::get() const
{
   return m_value;
}

std::string CCurtain::format() const
{
   shared::CDataContainer yadomsCommand;

   switch(m_value)
   {
   case kOpen: yadomsCommand.set("cmd", CStandardValues::Open); break;
   case kClose: yadomsCommand.set("cmd", CStandardValues::Close); break;
   case kStop: yadomsCommand.set("cmd", CStandardValues::Stop); break;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
      break;
   }

   return yadomsCommand.serialize();
}

} } } } // namespace shared::plugin::yadomsApi::commands

