#include "stdafx.h"
#include "Switch.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{
   DECLARE_ENUM_IMPLEMENTATION_NESTED(CSwitch::EState, EState,
      (Off)
      (On)
      (Dim)
   );
   
   CSwitch::CSwitch(const shared::CDataContainer& command)
   :State(EState::kOff), DimLevel(0)
   {
      try
      {
         State = command.get<EState>("command");
         if (State == EState::kDim)
            DimLevel = command.get<int>("level");
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

   CSwitch::CSwitch(EState state, int dimLevel)
      :State(state), DimLevel(dimLevel)
   {
   }

   CSwitch::~CSwitch()
   {
   }

  

   std::string CSwitch::format() const
   {
      shared::CDataContainer yadomsCommand;
      yadomsCommand.set("command", State);
      if (State == EState::kDim)
         yadomsCommand.set("level", DimLevel);
      return yadomsCommand.serialize();
   }

} } } } // namespace shared::plugin::yadomsApi::commands

