#pragma once
#include "IMessageHandler.h"
#include "../response/ReceivedMessage.h"

namespace message::smart_ack
{
    //--------------------------------------------------------------
    /// \brief	EnOcean Smart Ack read client mailbox status command
    //--------------------------------------------------------------
    class CReadClientMailboxStatusCommand final
    {
    public:
        enum MailboxStatus
        {
            EMPTY,
            FULL,
            DOESNT_EXIST
        };

        explicit CReadClientMailboxStatusCommand(const boost::shared_ptr<IMessageHandler>& messageHandler);
        ~CReadClientMailboxStatusCommand() = default;

        void sendAndReceive(const std::string& clientId,
                            const std::string& controllerId);

        [[nodiscard]] MailboxStatus status() const;
        [[nodiscard]] static std::string toString(MailboxStatus mailboxStatus);

    private:
        void processAnswer(const response::CReceivedMessage& response,
                           const std::string& requestName);
        [[nodiscard]] static MailboxStatus toMailboxStatus(unsigned char byte);

        const boost::shared_ptr<IMessageHandler> m_messageHandler;

        MailboxStatus m_status = DOESNT_EXIST;
    };
} // namespace message
