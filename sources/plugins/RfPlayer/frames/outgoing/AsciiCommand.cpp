#include "stdafx.h"
#include "AsciiCommand.h"

namespace frames {
namespace outgoing {
   
   const std::string CAsciiCommand::generateCommandById(const std::string & protocol, const std::string & command, const std::string & id, const std::string & dim, const std::string & burst, const std::string & qualifier)
   {
      std::string idCommand = (boost::format("ID %1%") % id).str();
      return generateCommandByAddress(protocol, command, idCommand, dim, burst, qualifier);
   }

   //--------------------------------------------------------------
   /// \brief              Generate a command for a device addressed by pseudo x10 format
   /// \param [in] protocol            The protocol (BLYSS, x10, RTS, X2D868,...)
   /// \param [in] command             The command (ON, OFF, DIM, ASSOC, ...)
   /// \param [in] pseudoAddress       The pseudo x10 address ([A-P][1-15])
   /// \param [in] dim                 The dimming percentage
   /// \param [in] burst               The burst value
   /// \param [in] qualitifer          The qualifier value
   //--------------------------------------------------------------
   const std::string CAsciiCommand::generateCommandByAddress(const std::string & protocol, const std::string & command, const std::string & pseudoAddress, const std::string & dim, const std::string & burst, const std::string & qualifier)
   {
      std::stringstream ss;
      ss << "ZIA++ " << command << " " << protocol << " " << pseudoAddress;

      if (!dim.empty())
         ss << " %" << dim;
      if (!burst.empty())
         ss << " BURST " << burst;
      if (!qualifier.empty())
         ss << " QUALITIFER " << qualifier;

      ss << "\r\n";
      return ss.str();
   }


      
} //namespace incoming
} //namespace frames