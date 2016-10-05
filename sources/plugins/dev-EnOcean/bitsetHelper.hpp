#pragma once


//--------------------------------------------------------------
/// \brief	Some bitset helper functions
//--------------------------------------------------------------
//TODO revoir cette fonction
template<size_t N>
std::vector<unsigned char> bitset_to_bytes(const std::bitset<N>& bs)
{
   std::vector<unsigned char> result((N + 7) >> 3);
   for (auto j = 0; j<int(N); j++)
      result[j >> 3] |= (bs[j] << (j & 7));
   return result;
}

inline boost::dynamic_bitset<> bitset_from_bytes(const std::vector<unsigned char>& buf)
{
   boost::dynamic_bitset<> bitset(buf.size() * sizeof(unsigned char));
   size_t index = 0;
   for (auto byte = buf.begin(); byte != buf.end(); ++byte)
      for (auto bit = 7; bit >= 0; --bit)
         bitset[index++] = *byte & 0x01 << bit ? true : false;
   return bitset;
}

