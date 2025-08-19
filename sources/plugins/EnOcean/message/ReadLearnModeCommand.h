#pragma once
#include "IMessageHandler.h"
#include "LearnMode.h"
#include "ResponseReceivedMessage.h"
#include <optional>

namespace message
{
    //--------------------------------------------------------------
    /// \brief	EnOcean Read learn mode command
    //--------------------------------------------------------------
    class CReadLearnModeCommand final
    {
    public:
        explicit CReadLearnModeCommand(const boost::shared_ptr<IMessageHandler>& messageHandler);
        ~CReadLearnModeCommand() = default;

        void sendAndReceive();

        [[nodiscard]] bool learnModeEnable() const;
        [[nodiscard]] std::optional<unsigned int> channel() const;

    private:
        void processAnswer(const CResponseReceivedMessage& response,
                           const std::string& requestName);

        const boost::shared_ptr<IMessageHandler> m_messageHandler;

        bool m_learnModeEnable = false;
        std::optional<unsigned int> m_channel;
    };
} // namespace message
