#pragma once
#include <shared/Exceptions/Exception.hpp>

namespace server { 
namespace database { 

   //--------------------------------------------------------------
   /// \class Exception for accessing NULL objects
   //--------------------------------------------------------------
   class CDatabaseException : public CException
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
   
} //namespace server 
} //namespace database 
