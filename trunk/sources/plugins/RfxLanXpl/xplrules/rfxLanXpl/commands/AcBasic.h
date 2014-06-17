#pragma once
#include "Field.hpp"

namespace xplrules { namespace rfxLanXpl { namespace commands
{
   //-----------------------------------------------------
   ///\brief The ac.basic command parser
   //-----------------------------------------------------
   class CAcBasic
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] command   Yadoms command, as JSON string
      ///\throw               shared::exception::CInvalidParameter if fail to parse command
      //-----------------------------------------------------
      CAcBasic(const std::string& command);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CAcBasic();

      //-----------------------------------------------------
      ///\brief               The command value
      //-----------------------------------------------------
      CField<std::string> Command;

      //-----------------------------------------------------
      ///\brief               The command value
      //-----------------------------------------------------
      CField<int> Level;
   };



} // namespace commands
} // namespace rfxLanXpl 
} // namespace xplrules

