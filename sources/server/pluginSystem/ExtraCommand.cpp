#include "stdafx.h"
#include "ExtraCommand.h"

namespace pluginSystem
{

   CExtraCommand::CExtraCommand(const std::string& command, const shared::CDataContainer & data)
      :m_command(command), m_data(data)
   {
   }

   CExtraCommand::~CExtraCommand()
   {
   }

   const std::string& CExtraCommand::getCommand() const
   {
      return m_command;
   }

   const shared::CDataContainer & CExtraCommand::getData() const
   {
      return m_data;
   }



} // namespace pluginSystem	
