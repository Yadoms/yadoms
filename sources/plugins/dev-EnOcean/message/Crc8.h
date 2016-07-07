#pragma once

namespace message
{
   //--------------------------------------------------------------
   /// \brief	                           Compute CRC8 on a buffer (from beginIt included to endIt excluded)
   /// \param[in] beginIt                 Start iterator (included)
   /// \param[in] endIt                   Stop iterator (not included)
   //--------------------------------------------------------------
   unsigned char computeCrc8(std::vector<unsigned char>::const_iterator beginIt,
                             std::vector<unsigned char>::const_iterator endIt);
} // namespace message


