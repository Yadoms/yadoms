#pragma once
#include "Exception.hpp"

//--------------------------------------------------------------
/// \class Exception for accessing NULL objects
//--------------------------------------------------------------
class CNullReferenceException : public CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   //--------------------------------------------------------------
   CNullReferenceException(const char * reference = NULL)
   {
      std::ostringstream s;
      s << "Null reference exception" << std::endl;
      if (reference)
         s << "Exception details : " << reference << std::endl << std::endl;
      m_message = s.str();
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CNullReferenceException() throw()
   {
   }
};
