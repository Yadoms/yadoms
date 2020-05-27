#pragma once

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	The small header description
      /// \note Just one byte for the part number, and one for the part count
      //--------------------------------------------------------------
      struct SmallHeader
      {
         SmallHeader()
            : m_partNumber(0),
              m_partCount(0)
         {
         }

         SmallHeader(unsigned char partNumber,
                     unsigned char partCount)
            : m_partNumber(partNumber),
              m_partCount(partCount)
         {
         }

         unsigned char m_partNumber;
         unsigned char m_partCount;

         static size_t size()
         {
            return sizeof(m_partNumber) + sizeof(m_partCount);
         }
      };
   }
} // namespace shared::communication


