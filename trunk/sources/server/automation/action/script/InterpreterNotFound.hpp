#pragma once

#include <shared/exception/Exception.hpp>

namespace automation { namespace action { namespace script
{

   //--------------------------------------------------------------
   /// \class Exception for not found script interpreter
   //--------------------------------------------------------------
   class CInterpreterNotFound : public shared::exception::CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CInterpreterNotFound(const std::string& scriptPath)
         :CException(std::string("Script interpreter not found for script file " + scriptPath))
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CInterpreterNotFound() throw()
      {
      }
   };

} } } // namespace automation::action::script

