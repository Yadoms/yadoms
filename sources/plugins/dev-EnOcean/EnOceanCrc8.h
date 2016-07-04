#pragma once

namespace EnOceanMessage
{
   //--------------------------------------------------------------
   /// \brief	                           Compute CRC8 on a buffer (from beginOffset included to endOffset excluded)
   /// \param[in] buffer                  The buffer
   /// \param[in] beginOffset             Start offset (included)
   /// \param[in] endOffset               Stop offset (not included)
   //--------------------------------------------------------------
   unsigned char computeCrc8(const std::vector<unsigned char>& buffer,
                             unsigned int beginOffset,
                             unsigned int endOffset);
} // namespace EnOceanMessage


