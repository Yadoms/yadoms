#pragma once
#include "IMessageHandler.h"
#include "../LearnMode.h"
#include "../response/ReceivedMessage.h"

namespace message::smart_ack
{
    //--------------------------------------------------------------
    /// \brief	EnOcean Smart Ack pairing command
    //--------------------------------------------------------------
    class CPairingCommand final
    {
    public:
        explicit CPairingCommand(const boost::shared_ptr<IMessageHandler>& messageHandler);
        ~CPairingCommand() = default;

        void sendAndReceive(bool enable,
                            LearnMode learnMode,
                            unsigned int timeoutMs = 0) const; // 0 is default timeout (60 000 ms)

    private:
        static void processAnswer(response::CReceivedMessage::EReturnCode returnCode,
                                  const std::string& requestName);

        const boost::shared_ptr<IMessageHandler> m_messageHandler;
    };
} // namespace message
