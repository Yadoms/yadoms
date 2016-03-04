#pragma once
#include <shared/exception/Exception.hpp>
#include <shared/StringExtension.h>

namespace database { 

   //--------------------------------------------------------------
   /// \class Exception for accessing NULL objects
   //--------------------------------------------------------------
   class CDatabaseException : public shared::exception::CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CDatabaseException(const std::string & errMsg)
          :CException(std::string(errMsg))
      {
      }

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CDatabaseException(const std::string & errMsg, const std::string & innerError)
          :CException(std::string(errMsg) + " " + std::string(innerError))
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CDatabaseException() throw()
      {
      }
   };
   
 
} //namespace database 
