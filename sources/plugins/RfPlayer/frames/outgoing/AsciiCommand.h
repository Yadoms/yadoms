#pragma once
#include "IOutgoingFrame.h"

namespace frames {
namespace outgoing {
   
   class CAsciiCommand
   {
      public:
         //--------------------------------------------------------------
         /// \brief              Generate a command ofr a device addressed by 32 bits ID
         /// \param [in] protocol            The protocol (BLYSS, x10, RTS, X2D868,...)
         /// \param [in] command             The command (ON, OFF, DIM, ASSOC, ...)
         /// \param [in] id                  The device ID (in case of use of 32bits address range)
         /// \param [in] dim                 The dimming percentage
         /// \param [in] burst               The burst value
         /// \param [in] qualifier           The qualifier value
         //--------------------------------------------------------------
         static const std::string generateCommandById(const std::string & protocol, const std::string & command, const std::string & id, const std::string & dim = "", const std::string & burst = "", const std::string & qualifier = "");

         //--------------------------------------------------------------
         /// \brief              Generate a command for a device addressed by pseudo x10 format
         /// \param [in] protocol            The protocol (BLYSS, x10, RTS, X2D868,...)
         /// \param [in] command             The command (ON, OFF, DIM, ASSOC, ...)
         /// \param [in] pseudoAddress       The pseudo x10 address ([A-P][1-15])
         /// \param [in] dim                 The dimming percentage
         /// \param [in] burst               The burst value
         /// \param [in] qualifier           The qualifier value
         //--------------------------------------------------------------
         static const std::string generateCommandByAddress(const std::string & protocol, const std::string & command, const std::string & pseudoAddress, const std::string & dim = "", const std::string & burst = "", const std::string & qualifier = "");
         
   };
} //namespace incoming
} //namespace frames

