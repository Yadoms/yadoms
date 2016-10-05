#pragma once


//--------------------------------------------------------------
/// \brief	Some bitset helper functions
//--------------------------------------------------------------
template<size_t N>
std::vector<unsigned char> bitset_to_bytes(const std::bitset<N>& bs)
{
   std::vector<unsigned char> result((N + 7) >> 3);
   for (int j = 0; j<int(N); j++)
      result[j >> 3] |= (bs[j] << (j & 7));
   return result;
}

template<size_t N>
std::bitset<N> bitset_from_bytes(const std::vector<unsigned char>& buf)
{
   assert(buf.size() == ((N + 7) >> 3));
   std::bitset<N> result;
   for (int j = 0; j<int(N); j++)
      result[j] = ((buf[j >> 3] >> (j & 7)) & 1);
   return result;
}

