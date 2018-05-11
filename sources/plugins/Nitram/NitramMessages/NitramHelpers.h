#pragma once

#include <shared/communication/Buffer.hpp>

unsigned char calculateCheckSum(const shared::communication::CByteBuffer& message, unsigned char size);
unsigned char readHeaderPayloadSize(unsigned char header);
bool readHeaderError(unsigned char header);
bool readHeaderFinish(unsigned char header);
unsigned char createHeader(unsigned char size,
                           bool error,
                           bool finish);