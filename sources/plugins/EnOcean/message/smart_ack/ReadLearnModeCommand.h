#pragma once
#include "IMessageHandler.h"
#include "../LearnMode.h"
#include "../response/ReceivedMessage.h"

namespace message::smart_ack
{
    //--------------------------------------------------------------
    /// \brief	EnOcean Smart Ack read learn mode command
    //--------------------------------------------------------------
    class CReadLearnModeCommand final
    {
    public:
        explicit CReadLearnModeCommand(const boost::shared_ptr<IMessageHandler>& messageHandler);
        ~CReadLearnModeCommand() = default;

        void sendAndReceive();

        [[nodiscard]] bool learnModeEnable() const;
        [[nodiscard]] LearnMode learnMode() const;

    private:
        void processAnswer(const response::CReceivedMessage& response,
                           const std::string& requestName);

        const boost::shared_ptr<IMessageHandler> m_messageHandler;

        bool m_learnModeEnable = false;
        LearnMode m_learnMode = LearnMode::SimpleLearnMode;
    };
} // namespace message
