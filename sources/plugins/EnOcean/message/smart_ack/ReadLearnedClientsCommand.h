#pragma once
#include "IMessageHandler.h"
#include "../response/ReceivedMessage.h"
#include "Client.h"

namespace message::smart_ack
{
    //--------------------------------------------------------------
    /// \brief	EnOcean Smart Ack read learn mode command
    //--------------------------------------------------------------
    class CReadLearnedClientsCommand final
    {
    public:
        explicit CReadLearnedClientsCommand(const boost::shared_ptr<IMessageHandler>& messageHandler);
        ~CReadLearnedClientsCommand() = default;

        void sendAndReceive();

        [[nodiscard]] std::vector<boost::shared_ptr<CClient>> clients() const;

    private:
        void processAnswer(const response::CReceivedMessage& response,
                           const std::string& requestName);
        std::vector<boost::shared_ptr<CClient>> unSerializeClients(const std::vector<unsigned char>& responseData);

        const boost::shared_ptr<IMessageHandler> m_messageHandler;

        std::vector<boost::shared_ptr<CClient>> m_clients;
    };
} // namespace message
