#pragma once

#include <shared/exception/Exception.hpp>

namespace automation { namespace action
{

   //--------------------------------------------------------------
   /// \class Exception for not found script interpreter
   //--------------------------------------------------------------
   class CScriptInterpreterNotFound : public shared::exception::CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CScriptInterpreterNotFound(const std::string& scriptPath)
         :CException(std::string("Script interpreter not found for script file " + scriptPath))
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CScriptInterpreterNotFound() throw()
      {
      }
   };

} } // namespace automation::action

