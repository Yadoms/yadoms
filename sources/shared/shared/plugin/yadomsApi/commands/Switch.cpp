#include "stdafx.h"
#include "Switch.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{
   CSwitch::CSwitch(const shared::CDataContainer& command)
      :SwitchLevel(0)
   {
      try
      {
         SwitchLevel = command.get<int>("command");
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         BOOST_ASSERT_MSG(false, "Invalid switch command");
         throw shared::exception::CInvalidParameter("Invalid switch command \"" + command.serialize() + "\" : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         BOOST_ASSERT_MSG(false, "Invalid switch command");
         throw shared::exception::CInvalidParameter("Invalid switch command \"" + command.serialize() + "\" : " + e.what());
      }
   }

   CSwitch::CSwitch(int switchLevel)
      :SwitchLevel(switchLevel)
   {
   }

   CSwitch::~CSwitch()
   {
   }  

   std::string CSwitch::format() const
   {
      shared::CDataContainer yadomsCommand;
      yadomsCommand.set("command", SwitchLevel);
      return yadomsCommand.serialize();
   }

} } } } // namespace shared::plugin::yadomsApi::commands

