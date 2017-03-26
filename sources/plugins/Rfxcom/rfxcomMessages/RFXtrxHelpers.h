#pragma once

#include <shared/communication/Buffer.hpp>

//--------------------------------------------------------------
/// \brief	This file provides helpers to use RFXtrx.h file (provided by RFXCom)
//--------------------------------------------------------------

// BYTE (used by RFXtrx.h) is a pure-Windows type, we need to redefine it
// to be cross-platform compatible
typedef unsigned char BYTE;

// Include the RFXtrx.h file provided by RFXCom
#include "RFXtrx.h"

// Macro helper to clear a sub-structure of RBUF.
// Use it as : MEMCLEAR(my_RBUF_instance.ICMND);
#define MEMCLEAR(structure) memset(&structure,0,sizeof(structure));

// Structures are defined in RFXtrx.h as it is difficult to get size of each message.
// Here is an workaround to get size of each message.
extern const RBUF dummyRbufToComputeSizes;
#define GET_RBUF_STRUCT_SIZE(structName) sizeof(dummyRbufToComputeSizes.structName)

// Macro helper to encode length (length = packet size except the packet byte)
#define ENCODE_PACKET_LENGTH(packet)((BYTE)GET_RBUF_STRUCT_SIZE(packet) - 1)

// The max size of a message
extern const size_t RFXMESSAGE_maxSize;


//--------------------------------------------------------------
/// \brief	                           Basic checks on received message, including subType
/// \param[in] rbuf                    Message received from Rfxcom
/// \param[in] rbufSize                Message size, received from Rfxcom
/// \param[in] expectedType            Expected message type (pType) : log and assert if not match
/// \param[in] expectedSubType         Expected message subtype (sType) : log if not match. Use DONT_CHECK_SUBTYPE to skip this check
/// \param[in] expectedSize            Expected message size : log if not match. Use DONT_CHECK_SIZE to skip this check
/// \param[in] expectedSeqNumber       Expected sequence number : log if not match. Use DONT_CHECK_SEQUENCE_NUMBER to skip this check
/// \throw shared::exception::CException if one of check fails
/// \note                              All checks are perform, even if one fails
//--------------------------------------------------------------
extern const BYTE DONT_CHECK_SUBTYPE;
extern const size_t DONT_CHECK_SIZE;
extern const unsigned int DONT_CHECK_SEQUENCE_NUMBER;
void CheckReceivedMessage(const RBUF& rbuf,
                          size_t rbufSize,
                          BYTE expectedType,
                          BYTE expectedSubType,
                          size_t expectedSize,
                          unsigned int expectedSeqNumber);

//--------------------------------------------------------------
/// \brief	                           Make a send buffer from RBUF structure
/// \param[in] rbuf                    RBUF structure pointer
/// \param[in] subStructureSize        RBUF Substructure size
/// \return                            Buffer
//--------------------------------------------------------------
shared::communication::CByteBuffer toBuffer(const RBUF& rbuf, size_t subStructureSize);

//--------------------------------------------------------------
/// \brief	                           Make a buffer queue from one buffer
/// \param[in] rbuf                    RBUF structure pointer
/// \param[in] subStructureSize        RBUF Substructure size
/// \return                            Buffer queue containing only one buffer
//--------------------------------------------------------------
boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > toBufferQueue(const RBUF& rbuf, size_t subStructureSize);

//--------------------------------------------------------------
/// \brief	                           Normalize battery level
/// \param[in] fromRfxcom              Battery level, as encoded in RFXCom messages
/// \return                            Battery level in %
//--------------------------------------------------------------
int NormalizeBatteryLevel(unsigned char fromRfxcom);

//--------------------------------------------------------------
/// \brief	                           Normalize signalPower level
/// \param[in] fromRfxcom              signalPower level, as encoded in RFXCom messages
/// \return                            signalPower level in %
//--------------------------------------------------------------
int NormalizesignalPowerLevel(unsigned char fromRfxcom);

//--------------------------------------------------------------
/// \brief	                           Normalize temperature
/// \param[in] fromRfxcomTemperatureHigh  Temperature (high byte), as encoded in RFXCom messages
/// \param[in] fromRfxcomTemperatureLow   Temperature (low byte), as encoded in RFXCom messages
/// \param[in] negative                Temperature sign (false:positive, true:negative)
/// \return                            Temperature in °C
//--------------------------------------------------------------
double NormalizeTemperature(unsigned char fromRfxcomTemperatureHigh, unsigned char fromRfxcomTemperatureLow, bool negative = false);

