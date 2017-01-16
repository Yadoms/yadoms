#include "stdafx.h"
#include "StringBuffer.h"

namespace shared
{
   namespace communication
   {

      CStringBuffer::CStringBuffer(std::size_t size) 
         : CBuffer<char>(size)
      {
      }

      CStringBuffer::CStringBuffer(const char* buffer, std::size_t size)
         : CBuffer<char>(buffer, size)
      {
      }   

      CStringBuffer::CStringBuffer(const std::string & buffer)
         : CBuffer<char>(buffer.c_str(), buffer.size())
      {
      }

      CStringBuffer::CStringBuffer(const std::vector<char>& src) 
         : CBuffer<char>(src)
      {
      }

      CStringBuffer::CStringBuffer(const CStringBuffer& src)
         : CBuffer<char>(src.toString().c_str(), src.toString().size())
      {
      }

      CStringBuffer::CStringBuffer(const CByteBuffer& src)
         : CBuffer<char>((const char*)src.begin(), src.size())
      {
      }

      CStringBuffer& CStringBuffer::operator =(const CStringBuffer& src)
      {
         CBuffer<char>::operator=( (CBuffer<char>)(src) );
         return *this;
      }
      
      CStringBuffer& CStringBuffer::operator =(const std::string& src)
      {
         CBuffer<char>::operator=( CStringBuffer(src) );
         return *this;
      }

      CStringBuffer::~CStringBuffer()
      {
      }
   
      const std::string CStringBuffer::toString() const
      {
         return std::string(begin());
      }
   }
} // namespace shared::communication


