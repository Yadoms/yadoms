#pragma once
#include "Esp3SendPacket.h"
#include "profiles/eep.h"

namespace message
{
   //--------------------------------------------------------------
   /// \brief	EnOcean send command message
   //--------------------------------------------------------------
   class CRadioErp1SendMessage : public CEsp3SendPacket
   {
   public:
      CRadioErp1SendMessage(CRorgs::ERorgIds rorg,
                            const std::string& senderId,
                            const std::string& destinationId,
                            unsigned char status);
      virtual ~CRadioErp1SendMessage();

      void userData(const std::vector<unsigned char>& userData);

      boost::shared_ptr<const std::vector<unsigned char>> buffer() override;

   private:
      CRorgs::ERorgIds m_rorg;
      const std::string m_senderId;
      const std::string m_destinationId;
      std::vector<unsigned char> m_userData;
      unsigned char m_status;
   };
} // namespace message
