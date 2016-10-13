#pragma once
#include <shared/exception/Exception.hpp>


namespace database { namespace common { namespace versioning { 

   //--------------------------------------------------------------
   /// \class Standard exception with some text
   //--------------------------------------------------------------
   class CVersionException : public shared::exception::CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in]  message             Exception message
      //--------------------------------------------------------------
      explicit CVersionException(const char * message)
         :CException(message)
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CVersionException() throw()
      {
      }
   };

} //namespace versioning
} //namespace common
} //namespace database 

