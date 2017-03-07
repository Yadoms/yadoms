#include "stdafx.h"

#include "../../../../sources/shared/shared/communication/IBufferLogger.h"


class CBufferLoggerMock : public shared::communication::IBufferLogger
{
public:
   explicit CBufferLoggerMock()
   {
   }

   virtual ~CBufferLoggerMock()
   {
   }

   void logReceived(const shared::communication::CByteBuffer& data) override
   {
   }

   void logSent(const shared::communication::CByteBuffer& data) override
   {
   }
};

