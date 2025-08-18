#pragma once
#include "IMessageHandler.h"
#include "ResponseReceivedMessage.h"
#include "SmartAckClient.h"

namespace message
{
    //--------------------------------------------------------------
    /// \brief	EnOcean Smart Ack read learn mode command
    //--------------------------------------------------------------
    class CSmartAckReadLearnedClientsCommand final
    {
    public:
        explicit CSmartAckReadLearnedClientsCommand(const boost::shared_ptr<IMessageHandler>& messageHandler);
        ~CSmartAckReadLearnedClientsCommand() = default;

        void sendAndReceive();

        [[nodiscard]] std::vector<boost::shared_ptr<CSmartAckClient>> clients() const;

    private:
        void processAnswer(const CResponseReceivedMessage& response,
                           const std::string& requestName);
        std::vector<boost::shared_ptr<CSmartAckClient>> unSerializeClients(const std::vector<unsigned char>& responseData);

        const boost::shared_ptr<IMessageHandler> m_messageHandler;

        std::vector<boost::shared_ptr<CSmartAckClient>> m_clients;
    };
} // namespace message
