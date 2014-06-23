#include "stdafx.h"
#include "Switch.h"
#include "../StandardValues.h"
#include <shared/DataContainer.h>
#include <shared/exception/InvalidParameter.hpp>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CSwitch::CSwitch(const std::string& command)
   :m_state(kOff), m_dimLevel(0)
{
   try
   {
	   shared::CDataContainer yadomsCommandTree(command);
	   std::string cmdState = yadomsCommandTree.get<std::string>("command");

      if (cmdState == CStandardValues::Off)
         m_state = kOff;
      else if (cmdState == CStandardValues::On)
         m_state = kOn;
      else if (cmdState == CStandardValues::Dim)
      {
         m_state = kDim;
		 m_dimLevel = yadomsCommandTree.get<int>("level");
      }
      else
      {
         BOOST_ASSERT_MSG(false, "Wrong switch command value");
         throw shared::exception::CInvalidParameter("Invalid switch command \"" + command + "\" : value out of range");
      }
   }
   catch (boost::property_tree::ptree_bad_path& e)
   {
      BOOST_ASSERT_MSG(false, "Invalid switch command");
      throw shared::exception::CInvalidParameter("Invalid switch command \"" + command + "\" : " + e.what());
   }
   catch (boost::property_tree::ptree_bad_data& e)
   {
      BOOST_ASSERT_MSG(false, "Invalid switch command");
      throw shared::exception::CInvalidParameter("Invalid switch command \"" + command + "\" : " + e.what());
   }
}

CSwitch::~CSwitch()
{
}

CSwitch::EState CSwitch::getState() const
{
   return m_state;
}

int CSwitch::getDimLevel() const
{
   return m_dimLevel;
}

} } } } // namespace shared::plugin::yadomsApi::commands

