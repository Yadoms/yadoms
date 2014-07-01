#include "stdafx.h"
#include "RFXtrxHelpers.h"

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