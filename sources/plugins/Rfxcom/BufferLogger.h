#pragma once
#include "IBufferLogger.h"

//--------------------------------------------------------------
/// \brief	This class manage a buffer logger
//--------------------------------------------------------------
class CBufferLogger : public IBufferLogger
{  
public:
   //--------------------------------------------------------------
   /// \brief	            Constructor
   //--------------------------------------------------------------
   CBufferLogger();

   //--------------------------------------------------------------
   /// \brief	            Destructor
   //--------------------------------------------------------------
   virtual ~CBufferLogger();

   // IBufferLogger implementation
   virtual void logReceived(const CByteBuffer& data);
   virtual void logSent(const CByteBuffer& data);
   virtual std::string msgToString(const CByteBuffer& data) const;
   // [END] IBufferLogger implementation
};
