#include "stdafx.h"
#include "Curtain.h"
#include "../StandardValues.h"
#include <shared/exception/InvalidParameter.hpp>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{
   DECLARE_ENUM_IMPLEMENTATION_NESTED(CCurtain::, Command,
      (Stop)
      (Open)
      (Close)
   );  

   CCurtain::CCurtain(const shared::CDataContainer& command)
      :m_value(ECommand::kStop)
   {
      try
      {
         m_value = command.get<ECommand>("cmd");
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
      :m_value(command)
   {
   }

   const CField<CCurtain::ECommand> & CCurtain::get() const
   {
      return m_value;
   }

   std::string CCurtain::format() const
   {
      shared::CDataContainer yadomsCommand;
      yadomsCommand.set("cmd", m_value());
      return yadomsCommand.serialize();
   }

} } } } // namespace shared::plugin::yadomsApi::commands

