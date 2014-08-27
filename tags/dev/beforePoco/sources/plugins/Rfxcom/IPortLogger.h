#pragma once
#include "Buffer.hpp"

//--------------------------------------------------------------
/// \brief	This class manage a communication port logger
//--------------------------------------------------------------
class IPortLogger
{  
public:
   //--------------------------------------------------------------
   /// \brief	            Destructor
   //--------------------------------------------------------------
   virtual ~IPortLogger() {}

   //--------------------------------------------------------------
   /// \brief	            Log recieved data
   /// \param[in] log      Buffer to log
   //--------------------------------------------------------------
   virtual void logReceived(const CByteBuffer& data) = 0;

   //--------------------------------------------------------------
   /// \brief	            Log sent data
   /// \param[in] log      Buffer to log
   //--------------------------------------------------------------
   virtual void logSent(const CByteBuffer& data) = 0;
};



