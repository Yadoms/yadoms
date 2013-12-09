#pragma once
#include "tools/Exceptions/Exception.h"


//--------------------------------------------------------------
/// \class Standard excption with some text
//--------------------------------------------------------------
class CSQLiteVersionException : public CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  message             Excpetion message
   //--------------------------------------------------------------
   CSQLiteVersionException(const char * message)
      :CException(message)
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   //TODO : pour Seb 
   /*
   
   virtual ~CSQLiteVersionException()
   {
   }
   */
};
