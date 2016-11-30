#pragma once
#include "Header.h"

namespace message
{
   //--------------------------------------------------------------
   /// \brief	EnOcean ESP3 received message
   ///
   /// This class manages an ESP3 EnOcean message, when receiving.
   //--------------------------------------------------------------
   class CEsp3ReceivedPacket
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] receivedBuffer          The received buffer
      //--------------------------------------------------------------
      explicit CEsp3ReceivedPacket(const std::vector<unsigned char>& receivedBuffer);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CEsp3ReceivedPacket();

      const CHeader& header() const;
      const std::vector<unsigned char>& data() const;
      const std::vector<unsigned char>& optional() const;

      const std::vector<unsigned char>& buffer() const;

   private:
      const std::vector<unsigned char> m_receivedBuffer;
      const CHeader m_header;
      const std::vector<unsigned char> m_data;
      const std::vector<unsigned char> m_optional;
   };
} // namespace message
