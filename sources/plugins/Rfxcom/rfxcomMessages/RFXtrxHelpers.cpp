#include "stdafx.h"
#include "RFXtrxHelpers.h"
#include <shared/exception/Exception.hpp>
#include <shared/Log.h>
#include "../ISequenceNumber.h"
#include "../NullSequenceNumber.h"

//--------------------------------------------------------------
/// \brief	The code of RFXtrx.h helpers
//--------------------------------------------------------------

const RBUF dummyRbufToComputeSizes = {{0}};

const size_t RFXMESSAGE_maxSize = sizeof(dummyRbufToComputeSizes);

const BYTE DONT_CHECK_SUBTYPE = std::numeric_limits<BYTE>::max();
const size_t DONT_CHECK_SIZE = std::numeric_limits<std::size_t>::max();
const boost::shared_ptr<ISequenceNumber> DONT_CHECK_SEQUENCE_NUMBER = boost::make_shared<CNullSequenceNumber>();

shared::communication::CByteBuffer toBuffer(const RBUF& rbuf, size_t subStructureSize)
{
   shared::communication::CByteBuffer buffer(subStructureSize);
   memcpy(buffer.begin(), &rbuf, subStructureSize);
   return buffer;
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> toBufferQueue(const RBUF& rbuf,
                                                                                size_t subStructureSize)
{
   auto buffers = boost::make_shared<std::queue<shared::communication::CByteBuffer>>();
   buffers->push(toBuffer(rbuf, subStructureSize));
   return buffers;
}

void CheckReceivedMessage(const RBUF& rbuf,
                          size_t rbufSize,
                          BYTE expectedType,
                          BYTE expectedSubType,
                          size_t expectedSize,
                          boost::shared_ptr<ISequenceNumber> sequenceNumberChecker)
{
   if (rbufSize != expectedSize && expectedSize != DONT_CHECK_SIZE)
      throw shared::exception::CException((boost::format("Wrong message size, received : %1% byte(s), expected : %2%") % rbufSize % expectedSize).str());

   if (expectedType != rbuf.RXRESPONSE.packettype)
      throw shared::exception::CException((boost::format("Wrong message type, received : %1%, expected : %2%") % static_cast<unsigned int>(rbuf.RXRESPONSE.packettype) % expectedType).str());

   if (expectedSubType != DONT_CHECK_SUBTYPE && expectedSubType != rbuf.RXRESPONSE.subtype)
      throw shared::exception::CException((boost::format("Wrong message subtype, received : %1%, expected : %2%") % static_cast<unsigned int>(rbuf.RXRESPONSE.subtype) % expectedSubType).str());

   if (expectedSize != DONT_CHECK_SIZE && (expectedSize - 1) != rbuf.RXRESPONSE.packetlength)
      throw shared::exception::CException((boost::format("Wrong message length, received : %1%, expected : %2% (total_message_size - 1)") % static_cast<unsigned int>(rbuf.RXRESPONSE.packetlength) % (expectedSize - 1)).str());

   if (!sequenceNumberChecker->isExpected(rbuf.RXRESPONSE.seqnbr))
   {
      YADOMS_LOG(information) << "Wrong message sequence number, received : " << static_cast<unsigned int>(rbuf.RXRESPONSE.seqnbr) << ", expected : " << static_cast<unsigned int>(sequenceNumberChecker->last());
      sequenceNumberChecker->reset(rbuf.RXRESPONSE.seqnbr);
   }
}

int NormalizeBatteryLevel(unsigned char fromRfxcom)
{
   auto batteryLevel = (fromRfxcom + 1) * 10;
   return (batteryLevel == 10 ? 0 : batteryLevel); // 10% = empty ==> force it to 0%
}

int NormalizeRssiLevel(unsigned char fromRfxcom)
{
   return (fromRfxcom * 100 / 0x0F);
}

double NormalizeTemperature(unsigned char fromRfxcomTemperatureHigh, unsigned char fromRfxcomTemperatureLow, bool negative)
{
   auto temperature = static_cast<double>((fromRfxcomTemperatureHigh << 8) | fromRfxcomTemperatureLow) / 10.0;
   return (negative ? -temperature : temperature);
}
