#include "stdafx.h"
#include "SwitchOnOff.h"
#include "../StandardValues.h"
#include <shared/serialization/PTreeToJsonSerializer.h>
#include <shared/exception/InvalidParameter.hpp>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CSwitchOnOff::CSwitchOnOff(const std::string& command)
   :m_isOn(false)
{
   shared::serialization::CPtreeToJsonSerializer serializer;
   try
   {
      boost::property_tree::ptree yadomsCommandTree = serializer.deserialize(command);
      std::string cmdValue = yadomsCommandTree.get<std::string>("cmd");

      BOOST_ASSERT_MSG(cmdValue == CStandardValues::On || cmdValue == CStandardValues::Off, "Wrong switchOnOff command value");
      if (cmdValue != CStandardValues::On && cmdValue != CStandardValues::Off)
         throw shared::exception::CInvalidParameter("Invalid switchOnOff command \"" + command + "\" : value out of range");

      m_isOn = (cmdValue == CStandardValues::On);
   }
   catch (boost::property_tree::ptree_bad_path& e)
   {
      BOOST_ASSERT_MSG(false, "Invalid switchOnOff command");
      throw shared::exception::CInvalidParameter("Invalid switchOnOff command \"" + command + "\" : " + e.what());
   }
   catch (boost::property_tree::ptree_bad_data& e)
   {
      BOOST_ASSERT_MSG(false, "Invalid switchOnOff command");
      throw shared::exception::CInvalidParameter("Invalid switchOnOff command \"" + command + "\" : " + e.what());
   }
}

CSwitchOnOff::~CSwitchOnOff()
{
}

bool CSwitchOnOff::isOn() const
{
   return m_isOn;
}

} } } } // namespace shared::plugin::yadomsApi::commands

