#pragma once

#include "Exception.hpp"

namespace shared { namespace exception
{

   //--------------------------------------------------------------
   /// \class Exception when archive extraction fails
   //--------------------------------------------------------------
   class CExtract : public CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      explicit CExtract(const std::string& message)
         :CException(message)
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CExtract() throw()
      {
      }
   };

} } // namespace shared::exception
