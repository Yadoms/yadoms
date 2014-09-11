#include "stdafx.h"
#include "Switch.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CSwitch::CSwitch(const shared::CDataContainer& command)
   :m_switchLevel(0)
{
   try
   {
      m_switchLevel = NormalizeLevel(command.get<int>("level"));
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
   :m_switchLevel(NormalizeLevel(switchLevel))
{
}

CSwitch::~CSwitch()
{
}

int CSwitch::NormalizeLevel(int level)
{
   if (level > 100)
      return 100;
   if (level < 0)
      return 0;
   return level;
}

const CField<int>& CSwitch::switchLevel() const
{
   return m_switchLevel;
}

bool CSwitch::isOn() const
{
   return (m_switchLevel >= 50) ? true : false;
}

std::string CSwitch::format() const
{
   shared::CDataContainer yadomsCommand;
   yadomsCommand.set("level", switchLevel());
   return yadomsCommand.serialize();
}

} } } } // namespace shared::plugin::yadomsApi::commands

