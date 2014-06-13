#include "stdafx.h"
#include "SwitchDim.h"
#include "../StandardValues.h"
#include <shared/serialization/PTreeToJsonSerializer.h>
#include <shared/exception/InvalidParameter.hpp>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CSwitchDim::CSwitchDim(const std::string& command)
   :m_value(0)
{
   shared::serialization::CPtreeToJsonSerializer serializer;
   try
   {
      boost::property_tree::ptree yadomsCommandTree = serializer.deserialize(command);
      int dimValue = yadomsCommandTree.get<int>("dim");

      BOOST_ASSERT_MSG(dimValue >= 0 && dimValue <= 100, "switchDim command value out of range");
      if (dimValue < 0 || dimValue > 100)
         throw shared::exception::CInvalidParameter("Invalid switchDim command \"" + command + "\" : value out of range");

      m_value = dimValue;
   }
   catch (boost::property_tree::ptree_bad_path& e)
   {
      BOOST_ASSERT_MSG(false, "Invalid switchDim command");
      throw shared::exception::CInvalidParameter("Invalid switchDim command \"" + command + "\" : " + e.what());
   }
   catch (boost::property_tree::ptree_bad_data& e)
   {
      BOOST_ASSERT_MSG(false, "Invalid switchDim command");
      throw shared::exception::CInvalidParameter("Invalid switchDim command \"" + command + "\" : " + e.what());
   }
}

CSwitchDim::~CSwitchDim()
{
}

int CSwitchDim::getValue() const
{
   return m_value;
}

} } } } // namespace shared::plugin::yadomsApi::commands

