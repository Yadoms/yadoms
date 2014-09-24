#pragma once
#include "Buffer.hpp"

//--------------------------------------------------------------
/// \brief	This interface manage a raw buffer logger
//--------------------------------------------------------------
class IBufferLogger
{  
public:
   //--------------------------------------------------------------
   /// \brief	            Destructor
   //--------------------------------------------------------------
   virtual ~IBufferLogger() {}

   //--------------------------------------------------------------
   /// \brief	            Log buffer as recieved data
   /// \param[in] log      Buffer to log
   //--------------------------------------------------------------
   virtual void logReceived(const CByteBuffer& data) = 0;

   //--------------------------------------------------------------
   /// \brief	            Log buffer as sent data
   /// \param[in] log      Buffer to log
   //--------------------------------------------------------------
   virtual void logSent(const CByteBuffer& data) = 0;

   //--------------------------------------------------------------
   /// \brief	            Adapt a buffer to be loggable
   /// \param [in] data    data
   /// \return             Formatted buffer
   //--------------------------------------------------------------
   virtual std::string msgToString(const CByteBuffer& data) const = 0;
};



