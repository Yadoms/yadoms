#pragma once
#include <shared/exception/Exception.hpp>


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
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CDatabaseException() throw()
      {
      }
   };
   
 
} //namespace database 
