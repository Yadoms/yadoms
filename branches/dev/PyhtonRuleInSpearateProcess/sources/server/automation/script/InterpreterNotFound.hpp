#pragma once

#include <shared/exception/Exception.hpp>

namespace automation { namespace script
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
      CInterpreterNotFound(const std::string& scriptName)
         :CException(std::string("Script interpreter not found for script " + scriptName))
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CInterpreterNotFound() throw()
      {
      }
   };

} } // namespace automation::script

