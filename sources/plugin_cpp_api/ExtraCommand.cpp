#include "stdafx.h"
#include "ExtraCommand.h"

namespace plugin_cpp_api
{
   CExtraCommand::CExtraCommand(const toPlugin::ExtraCommand& msg)
      : m_command(msg.command()),
        m_data(msg.data())
   {
   }

   CExtraCommand::~CExtraCommand()
   {
   }

   const std::string& CExtraCommand::getCommand() const
   {
      return m_command;
   }

   const shared::CDataContainer& CExtraCommand::getData() const
   {
      return m_data;
   }
} // namespace plugin_cpp_api	


