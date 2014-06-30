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

// Macro helper to encode length (length = packet size except the packet byte)
#define ENCODE_PACKET_LENGTH(packet)((BYTE)packet##_size - 1)
