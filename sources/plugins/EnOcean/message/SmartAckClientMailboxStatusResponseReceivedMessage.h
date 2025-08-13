#pragma once
#include "ResponseReceivedMessage.h"


namespace message
{
    //--------------------------------------------------------------
    /// \brief	EnOcean dongle Smart Ack learned clients RESPONSE message
    ///
    /// This class manages a Smart Ack learned clients RESPONSE EnOcean message.
    //--------------------------------------------------------------
    class CSmartAckClientMailboxStatusResponseReceivedMessage final
    {
    public:
        enum MailboxStatus
        {
            EMPTY,
            FULL,
            DOESNT_EXIST
        };

        //--------------------------------------------------------------
        /// \brief	                           Constructor
        /// \param[in] response                The response message
        //--------------------------------------------------------------
        explicit CSmartAckClientMailboxStatusResponseReceivedMessage(const boost::shared_ptr<const CResponseReceivedMessage>& response);
        ~CSmartAckClientMailboxStatusResponseReceivedMessage() = default;

        MailboxStatus status() const;
        static std::string toString(MailboxStatus mailboxStatus);

    private:
        static MailboxStatus toMailboxStatus(unsigned char byte);

        MailboxStatus m_status;
    };
} // namespace message
