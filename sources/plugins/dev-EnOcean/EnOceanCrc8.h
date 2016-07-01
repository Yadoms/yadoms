#pragma once

namespace EnOceanMessage
{
   //--------------------------------------------------------------
   /// \brief	                           Compute CRC8 on a buffer (from beginOffset to endOffset all included)
   /// \param[in] buffer                  The buffer
   /// \param[in] beginOffset             Start offset
   /// \param[in] endOffset               Stop offset
   //--------------------------------------------------------------
   unsigned char computeCrc8(const std::vector<unsigned char>& buffer,
                             unsigned int beginOffset,
                             unsigned int endOffset);
} // namespace EnOceanMessage


