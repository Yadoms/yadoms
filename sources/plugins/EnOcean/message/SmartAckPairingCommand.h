#pragma once
#include "IMessageHandler.h"
#include "LearnMode.h"
#include "ResponseReceivedMessage.h"

namespace message
{
    //--------------------------------------------------------------
    /// \brief	EnOcean Smart Ack pairing command
    //--------------------------------------------------------------
    class CSmartAckPairingCommand final
    {
    public:
        explicit CSmartAckPairingCommand(const boost::shared_ptr<IMessageHandler>& messageHandler);
        ~CSmartAckPairingCommand() = default;

        void sendAndReceive(bool enable,
                            LearnMode learnMode,
                            unsigned int timeoutMs = 0) const; // 0 is default timeout (60 000 ms)

    private:
        static void processAnswer(CResponseReceivedMessage::EReturnCode returnCode,
                                  const std::string& requestName);

        const boost::shared_ptr<IMessageHandler> m_messageHandler;
    };
} // namespace message
