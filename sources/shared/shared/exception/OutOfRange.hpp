#pragma once

#include "Exception.hpp"

namespace shared { namespace exception
{

   //--------------------------------------------------------------
   /// \brief Exception for value out of range
   //--------------------------------------------------------------
   class COutOfRange : public CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      explicit COutOfRange(const std::string& message)
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
