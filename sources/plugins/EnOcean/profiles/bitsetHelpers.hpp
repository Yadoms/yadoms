#pragma once


//--------------------------------------------------------------
/// \brief	Some bitset helper functions
//--------------------------------------------------------------


inline std::vector<unsigned char> bitset_to_bytes(const boost::dynamic_bitset<>& bs)
{
   std::vector<unsigned char> byteArray(bs.size() / 8);
   for (std::vector<unsigned char>::size_type bytePos = 0; bytePos < byteArray.size(); ++bytePos)
   {
      unsigned char byte = 0;
      for (auto bitPos = 0; bitPos < 8; ++bitPos)
         if (bs[bytePos * 8 + bitPos])
            byte |= 0x80 >> bitPos;
      byteArray[bytePos] = byte;
   }

   return byteArray;
}

inline unsigned char bitset_to_byte(const boost::dynamic_bitset<>& bs)
{
   unsigned char byte = 0;
   for (auto bitPos = 0; bitPos < 8; ++bitPos)
      if (bs[bitPos])
         byte |= 0x80 >> bitPos;
   return byte;
}

inline void bitset_insert(boost::dynamic_bitset<>& bitset,
                          size_t position,
                          size_t size,
                          unsigned int value)
{
   for (size_t index = 0; index < size; ++index)
      bitset[position + index] = (value & (0x00000001 << (size - index - 1))) ? true : false;
}

inline void bitset_insert(boost::dynamic_bitset<>& bitset,
                          size_t position,
                          bool value)
{
   bitset[position] = value;
}

inline boost::dynamic_bitset<> bitset_from_bytes(const std::vector<unsigned char>& buf)
{
   boost::dynamic_bitset<> bitset(buf.size() * 8);
   size_t index = 0;
   for (const auto byte : buf)
      for (auto bit = 7; bit >= 0; --bit)
         bitset[index++] = (byte & 0x01 << bit) ? true : false;
   return bitset;
}

inline boost::dynamic_bitset<> bitset_from_byte(unsigned char byte)
{
   boost::dynamic_bitset<> bitset(sizeof(byte) * 8);
   size_t index = 0;
   for (auto bit = 7; bit >= 0; --bit)
      bitset[index++] = (byte & 0x01 << bit) ? true : false;
   return bitset;
}

inline unsigned int bitset_extract(const boost::dynamic_bitset<>& bitset,
                                   size_t position,
                                   size_t size)
{
   unsigned int value = 0;
   if (size > (sizeof(value) * 8))
      throw std::out_of_range((boost::format("bitset extraction error, size is too big : %1%") % size).str());
   for (size_t index = 0; index < size; ++index)
      if (bitset[position + index])
         value |= 1 << (size - index - 1);
   return value;
}

