#include "stdafx.h"
#include "RFXtrxHelpers.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

//--------------------------------------------------------------
/// \brief	The code of RFXtrx.h helpers
//--------------------------------------------------------------

const RBUF dummyRbufToComputeSizes = { 0 };

const size_t RFXMESSAGE_maxSize = sizeof(dummyRbufToComputeSizes);

const unsigned int DONT_CHECK_SEQUENCE_NUMBER = UINT_MAX;

void CheckReceivedMessage(const RBUF& rbuf, size_t rbufSize, BYTE expectedType, size_t expectedSize, unsigned int expectedSeqNumber)
{
   if (rbufSize != expectedSize)
      throw shared::exception::CException((boost::format("Wrong message size, received : %1% byte(s), expected : %2%") % rbufSize % expectedSize).str());

   if (rbuf.RXRESPONSE.packettype != expectedType)
      throw shared::exception::CException((boost::format("Wrong message type, received : %1%, expected : %2%") % static_cast<unsigned int>(rbuf.RXRESPONSE.packettype) % expectedType).str());

   if (rbuf.RXRESPONSE.packetlength != (expectedSize - 1))
      throw shared::exception::CException((boost::format("Wrong message length, received : %1%, expected : %2% (total_message_size - 1)") % static_cast<unsigned int>(rbuf.RXRESPONSE.packetlength) % (expectedSize - 1)).str());

   if (expectedSeqNumber != DONT_CHECK_SEQUENCE_NUMBER && rbuf.RXRESPONSE.seqnbr != expectedSeqNumber)
      YADOMS_LOG(warning) << "Wrong message sequence number, received : " << static_cast<unsigned int>(rbuf.RXRESPONSE.seqnbr) << ", expected : " << expectedSeqNumber;
}

shared::communication::CByteBuffer toBuffer(const RBUF& rbuf, size_t subStructureSize)
{
   shared::communication::CByteBuffer buffer(subStructureSize);
   memcpy(buffer.begin(), &rbuf, subStructureSize);
   return buffer;
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > toBufferQueue(const RBUF& rbuf, size_t subStructureSize)
{
   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > buffers(new std::queue<shared::communication::CByteBuffer>);
   buffers->push(toBuffer(rbuf, subStructureSize));
   return buffers;
}

void CheckReceivedMessage(const RBUF& rbuf, size_t rbufSize, BYTE expectedType, BYTE expectedSubType, size_t expectedSize, unsigned int expectedSeqNumber)
{
   CheckReceivedMessage(rbuf, rbufSize, expectedType, expectedSize, expectedSeqNumber);

   // Add subType check
   if (rbuf.RXRESPONSE.subtype != expectedSubType)
      throw shared::exception::CException((boost::format("Wrong message subtype, received : %1%, expected : %2%") % static_cast<unsigned int>(rbuf.RXRESPONSE.subtype) % expectedSubType).str());
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
   double temperature = static_cast<double>((fromRfxcomTemperatureHigh << 8) | fromRfxcomTemperatureLow) / 10.0;
   return (negative ? -temperature : temperature);
}
