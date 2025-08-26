#include "stdafx.h"
#include "SmartAckReadLearnedClientsCommand.h"

#include <ProtocolException.hpp>

#include "SmartAckCommandSendMessage.h"
#include "shared/Log.h"

namespace message
{
    CSmartAckReadLearnedClientsCommand::CSmartAckReadLearnedClientsCommand(const boost::shared_ptr<IMessageHandler>& messageHandler)
        : m_messageHandler(messageHandler)
    {
    }

    void CSmartAckReadLearnedClientsCommand::sendAndReceive()
    {
        YADOMS_LOG(information) << "Read Smart Ack learned clients...";

        CSmartAckCommandSendMessage sendMessage(CSmartAckCommandSendMessage::SA_RD_LEARNEDCLIENTS);

        boost::shared_ptr<const CEsp3ReceivedPacket> answer;
        if (!m_messageHandler->send(sendMessage,
                                    [](const boost::shared_ptr<const CEsp3ReceivedPacket>& esp3Packet)
                                    {
                                        if (esp3Packet->header().packetType() == RESPONSE)
                                            return true;
                                        YADOMS_LOG(warning) << "Unexpected message received : wrong packet type : " << esp3Packet->header().
                                            packetType();
                                        return false;
                                    },
                                    [&](boost::shared_ptr<const CEsp3ReceivedPacket> esp3Packet)
                                    {
                                        answer = std::move(esp3Packet);
                                    }))
            throw CProtocolException("Timeout waiting answer");

        processAnswer(response::CReceivedMessage(answer),
                      "SA_RD_LEARNEDCLIENTS");
    }

    std::vector<boost::shared_ptr<CSmartAckClient>> CSmartAckReadLearnedClientsCommand::clients() const
    {
        return m_clients;
    }

    void CSmartAckReadLearnedClientsCommand::processAnswer(const response::CReceivedMessage& response,
                                                           const std::string& requestName)
    {
        if (response.returnCode() != response::CReceivedMessage::RET_OK)
            throw CProtocolException("  ==> " + requestName + " request returned " + response::CReceivedMessage::toString(response.returnCode()));

        m_clients = unSerializeClients(response.responseData());

        constexpr unsigned int clientSize = 9;

        if (response.responseData().size() % clientSize != 0)
            throw CProtocolException(
                "Learned client response : invalid received data size " + std::to_string(response.responseData().size()) +
                ", expected multiple of " + std::to_string(clientSize));

        std::vector<boost::shared_ptr<CSmartAckClient>> clients(response.responseData().size() / clientSize);
        for (auto client = 0u; client < m_clients.size(); ++client)
        {
            m_clients.emplace_back(std::make_shared<CSmartAckClient>(
                deviceIdToString(response.responseData()[client * clientSize] << 24
                    | response.responseData()[client * clientSize + 1] << 16
                    | response.responseData()[client * clientSize + 2] << 8
                    | response.responseData()[client * clientSize + 3]),
                deviceIdToString(response.responseData()[client * clientSize + 4] << 24
                    | response.responseData()[client * clientSize + 5] << 16
                    | response.responseData()[client * clientSize + 6] << 8
                    | response.responseData()[client * clientSize + 7]),
                static_cast<unsigned int>(response.responseData()[client * clientSize + 8])).get());
        }

        if (m_clients.empty())
        {
            YADOMS_LOG(information) << "  ==> No Smart Ack learned clients";
            return;
        }

        YADOMS_LOG(information) << "  ==> Smart Ack learned clients : ";
        for (const auto& client : m_clients)
            YADOMS_LOG(information) << " - ID " << client->id()
                << " : controller ID " << client->controllerId()
                << ", mailbox Index " << client->mailboxIndex();
    }

    std::vector<boost::shared_ptr<CSmartAckClient>> CSmartAckReadLearnedClientsCommand::unSerializeClients(
        const std::vector<unsigned char>& responseData)
    {
        constexpr unsigned int clientSize = 9;

        if (responseData.size() % clientSize != 0)
            throw CProtocolException(
                "Learned client response : invalid received data size " + std::to_string(responseData.size()) +
                ", expected multiple of " + std::to_string(clientSize));

        std::vector<boost::shared_ptr<CSmartAckClient>> clients(responseData.size() / clientSize);
        for (auto client = 0u; client < m_clients.size(); ++client)
        {
            m_clients.emplace_back(std::make_shared<CSmartAckClient>(
                deviceIdToString(responseData[client * clientSize] << 24
                    | responseData[client * clientSize + 1] << 16
                    | responseData[client * clientSize + 2] << 8
                    | responseData[client * clientSize + 3]),
                deviceIdToString(responseData[client * clientSize + 4] << 24
                    | responseData[client * clientSize + 5] << 16
                    | responseData[client * clientSize + 6] << 8
                    | responseData[client * clientSize + 7]),
                static_cast<unsigned int>(responseData[client * clientSize + 8])).get());
        }

        return clients;
    }
} // namespace message
