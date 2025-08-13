#pragma once
#include "ResponseReceivedMessage.h"
#include "SmartAckClient.h"


namespace message
{
    //--------------------------------------------------------------
    /// \brief	EnOcean dongle Smart Ack learned clients RESPONSE message
    ///
    /// This class manages a Smart Ack learned clients RESPONSE EnOcean message.
    //--------------------------------------------------------------
    class CSmartAckLearnedClientsResponseReceivedMessage final
    {
    public:
        //--------------------------------------------------------------
        /// \brief	                           Constructor
        /// \param[in] response                The response message
        //--------------------------------------------------------------
        explicit CSmartAckLearnedClientsResponseReceivedMessage(const boost::shared_ptr<const CResponseReceivedMessage>& response);
        ~CSmartAckLearnedClientsResponseReceivedMessage() = default;

        std::vector<boost::shared_ptr<CSmartAckClient>> clients() const;

    private:
        std::vector<boost::shared_ptr<CSmartAckClient>> unSerializeClients(const std::vector<unsigned char>& responseData);

        std::vector<boost::shared_ptr<CSmartAckClient>> m_clients;
    };
} // namespace message
