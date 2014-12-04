#include "stdafx.h"
#include "RFXtrxHelpers.h"
#include <shared/Log.h>

//--------------------------------------------------------------
/// \brief	The code of RFXtrx.h helpers
//--------------------------------------------------------------

const RBUF dummyRbufToComputeSizes = { 0 };

const std::size_t RFXMESSAGE_maxSize = sizeof(dummyRbufToComputeSizes);

const unsigned int DONT_CHECK_SEQUENCE_NUMBER = UINT_MAX;

bool CheckReceivedMessage(const RBUF& rbuf, BYTE expectedType, size_t expectedSize, unsigned int expectedSeqNumber)
{
   bool ok = true;

   if (rbuf.RXRESPONSE.packettype != expectedType)
   {
      YADOMS_LOG(error) << "Wrong message type, received : " << rbuf.RXRESPONSE.packettype << ", expected : " << expectedType;
      BOOST_ASSERT_MSG(false, "Wrong message type");
      ok = false;
   }

   if (rbuf.RXRESPONSE.packetlength != (expectedSize - 1))
   {
      YADOMS_LOG(warning) << "Wrong message length, received : " << rbuf.RXRESPONSE.packetlength << ", expected : " << (expectedSize - 1) << " (total_message_size - 1)";
      ok = false;
   }

   if (expectedSeqNumber != DONT_CHECK_SEQUENCE_NUMBER && rbuf.RXRESPONSE.seqnbr != expectedSeqNumber)
   {
      YADOMS_LOG(warning) << "Wrong message sequence number, received : " << rbuf.RXRESPONSE.seqnbr << ", expected : " << expectedSeqNumber;
      ok = false;
   }

   return ok;
}

const shared::communication::CByteBuffer toBuffer(const RBUF& rbuf, std::size_t subStructureSize)
{
   shared::communication::CByteBuffer buffer(subStructureSize);
   memcpy(buffer.begin(), &rbuf, subStructureSize);
   return buffer;
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > toBufferQueue(const RBUF& rbuf, std::size_t subStructureSize)
{
   boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > buffers(new std::queue<const shared::communication::CByteBuffer>);
   buffers->push(toBuffer(rbuf, subStructureSize));
   return buffers;
}

bool CheckReceivedMessage(const RBUF& rbuf, BYTE expectedType, BYTE expectedSubType, size_t expectedSize, unsigned int expectedSeqNumber)
{
   bool ok = CheckReceivedMessage(rbuf, expectedType, expectedSize, expectedSeqNumber);

   // Add subType check
   if (rbuf.RXRESPONSE.subtype != expectedSubType)
   {
      YADOMS_LOG(warning) << "Wrong message subtype, received : " << rbuf.RXRESPONSE.subtype << ", expected : " << expectedSubType;
      ok = false;
   }

   return ok;
}

int NormalizeBatteryLevel(unsigned char fromRfxcom)
{
   int batteryLevel = (fromRfxcom + 1) * 10;
   return (batteryLevel == 10 ? 0 : batteryLevel);  // 10% = empty ==> force it to 0%
}

int NormalizeRssiLevel(unsigned char fromRfxcom)
{
   return (fromRfxcom * 100 / 0x0F);
}

double NormalizeTemperature(unsigned char fromRfxcomTemperatureHigh, unsigned char fromRfxcomTemperatureLow, bool negative)
{
   double temperature = (double)((fromRfxcomTemperatureHigh << 8) | fromRfxcomTemperatureLow) / 10.0;
   return (negative ? -temperature : temperature);
}
