#include "stdafx.h"
#include "SmartAckLearnedClientsResponseReceivedMessage.h"

#include <ProtocolException.hpp>

#include "shared/Log.h"

namespace message
{
    CSmartAckLearnedClientsResponseReceivedMessage::CSmartAckLearnedClientsResponseReceivedMessage(
        const boost::shared_ptr<const CResponseReceivedMessage>& response)
        : m_clients(unSerializeClients(response->responseData()))
    {
    }

    std::vector<boost::shared_ptr<CSmartAckClient>> CSmartAckLearnedClientsResponseReceivedMessage::unSerializeClients(
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

    std::vector<boost::shared_ptr<CSmartAckClient>> CSmartAckLearnedClientsResponseReceivedMessage::clients() const
    {
        return m_clients;
    }
} // namespace message
