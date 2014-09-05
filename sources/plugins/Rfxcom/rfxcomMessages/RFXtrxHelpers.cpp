#include "stdafx.h"
#include "RFXtrxHelpers.h"
#include <shared/Log.h>

//--------------------------------------------------------------
/// \brief	The code of RFXtrx.h helpers
//--------------------------------------------------------------

static const RBUF dummyRbufToComputeSizes = { 0 };
const std::size_t ICMND_size = sizeof(dummyRbufToComputeSizes.ICMND);
const std::size_t IRESPONSE_size = sizeof(dummyRbufToComputeSizes.IRESPONSE);
const std::size_t RXRESPONSE_size = sizeof(dummyRbufToComputeSizes.RXRESPONSE);
const std::size_t LIGHTING1_size = sizeof(dummyRbufToComputeSizes.LIGHTING1);
const std::size_t LIGHTING2_size = sizeof(dummyRbufToComputeSizes.LIGHTING2);
const std::size_t LIGHTING3_size = sizeof(dummyRbufToComputeSizes.LIGHTING3);
const std::size_t LIGHTING4_size = sizeof(dummyRbufToComputeSizes.LIGHTING4);
const std::size_t LIGHTING5_size = sizeof(dummyRbufToComputeSizes.LIGHTING5);
const std::size_t LIGHTING6_size = sizeof(dummyRbufToComputeSizes.LIGHTING6);
const std::size_t CURTAIN1_size = sizeof(dummyRbufToComputeSizes.CURTAIN1);
const std::size_t TEMP_size = sizeof(dummyRbufToComputeSizes.TEMP);

const std::size_t RFXMESSAGE_maxSize = sizeof(dummyRbufToComputeSizes);

bool CheckReceivedMessage(const RBUF& rbuf, BYTE expectedType, size_t expectedSize, unsigned char expectedSeqNumber)
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

   if (rbuf.RXRESPONSE.seqnbr != expectedSeqNumber)
   {
      YADOMS_LOG(warning) << "Wrong message sequence number, received : " << rbuf.RXRESPONSE.seqnbr << ", expected : " << expectedSeqNumber;
      ok = false;
   }

   return ok;
}

bool CheckReceivedMessage(const RBUF& rbuf, BYTE expectedType, BYTE expectedSubType, size_t expectedSize, unsigned char expectedSeqNumber)
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