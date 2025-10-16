#pragma once
#include "IMessageHandler.h"
#include "LearnMode.h"
#include "response/ReceivedMessage.h"

namespace message
{
    //--------------------------------------------------------------
    /// \brief	EnOcean Write learn mode command
    //--------------------------------------------------------------
    class CWriteLearnModeCommand final
    {
    public:
        explicit CWriteLearnModeCommand(const boost::shared_ptr<IMessageHandler>& messageHandler);
        ~CWriteLearnModeCommand() = default;

        void sendAndReceive(bool enable,
                            unsigned int timeoutMs = 0) const; // 0 is default timeout (60 000 ms)

    private:
        static void processAnswer(const response::CReceivedMessage& response,
                                  const std::string& requestName);

        const boost::shared_ptr<IMessageHandler> m_messageHandler;
    };
} // namespace message
