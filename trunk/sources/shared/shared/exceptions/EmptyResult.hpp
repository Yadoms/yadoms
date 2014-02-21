#pragma once

#include "Exception.hpp"

namespace shared { namespace exception
{

   //--------------------------------------------------------------
   /// \class Exception for non implemented part of code
   //--------------------------------------------------------------
   class CEmptyResult : public CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CEmptyResult(const std::string & message)
         :CException(message)
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CEmptyResult() throw()
      {
      }
   };

} } // namespace shared::exception
