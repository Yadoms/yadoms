#pragma once

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

// Structures are defined as it is difficult to get size of each message.
// Here is an workaround to get size of each message.
extern const std::size_t ICMND_size;
extern const std::size_t IRESPONSE_size;
extern const std::size_t RXRESPONSE_size;
extern const std::size_t LIGHTING1_size;
extern const std::size_t LIGHTING2_size;
extern const std::size_t LIGHTING3_size;
extern const std::size_t LIGHTING4_size;
extern const std::size_t LIGHTING5_size;
extern const std::size_t LIGHTING6_size;
extern const std::size_t CURTAIN1_size;
extern const std::size_t TEMP_size;

// The max size of a message
extern const std::size_t RFXMESSAGE_maxSize;

// Macro helper to encode length (length = packet size except the packet byte)
#define ENCODE_PACKET_LENGTH(packet)((BYTE)packet##_size - 1)

//--------------------------------------------------------------
/// \brief	                           Basic checks on received message : type, size, sequence number
/// \param[in] rbuf                    Message received from Rfxcom
/// \param[in] expectedType            Expected message type (pType) : log and assert if not match
/// \param[in] expectedSize            Expected message size : log if not match
/// \param[in] expectedSeqNumber       Expected sequence number : log if not match. Use DONT_CHECK_SEQUENCE_NUMBER to skip this check
/// \return                            false if one of check fails
/// \note                              All checks are perform, even if one fails
//--------------------------------------------------------------
extern const unsigned int DONT_CHECK_SEQUENCE_NUMBER;
bool CheckReceivedMessage(const RBUF& rbuf, BYTE expectedType, size_t expectedSize, unsigned int expectedSeqNumber);

//--------------------------------------------------------------
/// \brief	                           Basic checks on received message, including subType
/// \param[in] rbuf                    Message received from Rfxcom
/// \param[in] expectedType            Expected message type (pType) : log and assert if not match
/// \param[in] expectedSubType         Expected message subtype (sType) : log if not match
/// \param[in] expectedSize            Expected message size : log if not match
/// \param[in] expectedSeqNumber       Expected sequence number : log if not match. Use DONT_CHECK_SEQUENCE_NUMBER to skip this check
/// \return                            false if one of check fails
/// \note                              All checks are perform, even if one fails
//--------------------------------------------------------------
bool CheckReceivedMessage(const RBUF& rbuf, BYTE expectedType, BYTE expectedSubType, size_t expectedSize, unsigned int expectedSeqNumber);
