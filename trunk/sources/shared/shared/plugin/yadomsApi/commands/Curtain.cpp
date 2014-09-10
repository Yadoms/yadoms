#include "stdafx.h"
#include "Curtain.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{
   DECLARE_ENUM_IMPLEMENTATION_NESTED(CCurtain::ECommand, ECommand,
      (Stop)
      (Open)
      (Close)
   );  

   CCurtain::CCurtain(const shared::CDataContainer& command)
      :m_command(ECommand::kStop)
   {
      try
      {
         m_command = command.get<ECommand>("cmd");
      }
      catch (shared::exception::CException & e)
      {
         BOOST_ASSERT_MSG(false, "Invalid curtain command");
         throw shared::exception::CInvalidParameter("Invalid curtain command \"" + command.serialize() + "\" : " + e.what());
      }
   }

   CCurtain::~CCurtain()
   {
   }

   CCurtain::CCurtain(ECommand command)
      :m_command(command)
   {
   }

   const CField<CCurtain::ECommand> & CCurtain::getCommand() const
   {
      return m_command;
   }

   std::string CCurtain::format() const
   {
      shared::CDataContainer yadomsCommand;
      yadomsCommand.set("cmd", m_command());
      return yadomsCommand.serialize();
   }

} } } } // namespace shared::plugin::yadomsApi::commands

