#include "stdafx.h"
#include "Curtain.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>

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
      {
         m_value = kOpen;
         return;
      }
      else if (cmdValue == CStandardValues::Close)
      {
         m_value = kClose;
         return;
      }
      else if (cmdValue == CStandardValues::Stop)
      {
         m_value = kStop;
         return;
      }
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

CCurtain::ECommand CCurtain::get() const
{
   return m_value;
}

} } } } // namespace shared::plugin::yadomsApi::commands

