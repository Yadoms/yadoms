#pragma once
#include "IMessageHandler.h"
#include "profiles/eep.h"
#include "radioErp1/SendMessage.h"

namespace message
{
   //--------------------------------------------------------------
   /// \brief	Generic message helpers
   //--------------------------------------------------------------
   class CMessageHelpers
   {
   public:
      CMessageHelpers() = delete;
      virtual ~CMessageHelpers() = default;

      static void sendMessage(CRorgs::ERorgIds rorgId,
                              const boost::shared_ptr<IMessageHandler>& messageHandler,
                              const std::string& senderId,
                              const std::string& targetId,
                              const boost::dynamic_bitset<>& userData,
                              const std::string& messageName);

      static void sendMessage(radioErp1::CSendMessage& message,
                              const boost::shared_ptr<IMessageHandler>& messageHandler,
                              const std::string& messageName = "unknown");
   };
} // namespace message
