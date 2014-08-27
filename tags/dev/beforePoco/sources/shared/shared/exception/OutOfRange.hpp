#pragma once

#include "Exception.hpp"

namespace shared { namespace exception
{

   //--------------------------------------------------------------
   /// \class Exception for value out of range
   //--------------------------------------------------------------
   class COutOfRange : public CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      COutOfRange(const std::string& message)
         :CException(std::string("Out of range " + message))
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~COutOfRange() throw()
      {
      }
   };

} } // namespace shared::exception
