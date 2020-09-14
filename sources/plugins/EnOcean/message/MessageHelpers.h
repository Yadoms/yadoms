#pragma once
#include "IMessageHandler.h"
#include "profiles/eep.h"

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
                              boost::shared_ptr<IMessageHandler> messageHandler,
                              const std::string& senderId,
                              const std::string& targetId,
                              const boost::dynamic_bitset<>& userData,
                              const std::string& messageName);
   };
} // namespace message
