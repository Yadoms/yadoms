#pragma once
#include <shared/Exceptions/Exception.hpp>


//--------------------------------------------------------------
/// \class Standard excption with some text
//--------------------------------------------------------------
class CSQLiteVersionException : public CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  message             Exception message
   //--------------------------------------------------------------
   CSQLiteVersionException(const char * message)
      :CException(message)
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CSQLiteVersionException() throw()
   {
   }
};
