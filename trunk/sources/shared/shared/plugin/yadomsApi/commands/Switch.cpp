#include "stdafx.h"
#include "Switch.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

   DECLARE_ENUM_IMPLEMENTATION(State,
      (Off)
      (On)
      (Dim)
   );

   CSwitch::CSwitch(const shared::CDataContainer& command)
   :m_state(EState::kOff), m_dimLevel(0)
   {
      try
      {
         m_state = command.get<EState>("command");
         if (m_state() == EState::kDim)
            m_dimLevel = command.get<int>("level");
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
      :m_state(state), m_dimLevel(dimLevel)
   {
   }

   CSwitch::~CSwitch()
   {
   }

   const CField<EState> & CSwitch::getState() const
   {
      return m_state;
   }

   const CField<int> & CSwitch::getDimLevel() const
   {
      return m_dimLevel;
   }

   std::string CSwitch::format() const
   {
      shared::CDataContainer yadomsCommand;
      yadomsCommand.set("command", m_state());
      if (m_state() == EState::kDim)
         yadomsCommand.set("level", m_dimLevel);
      return yadomsCommand.serialize();
   }

} } } } // namespace shared::plugin::yadomsApi::commands

