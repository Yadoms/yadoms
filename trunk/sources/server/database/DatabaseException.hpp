#pragma once
#include <shared/Exceptions/Exception.hpp>

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
