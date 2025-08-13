#include "stdafx.h"
#include "SmartAckClientMailboxStatusResponseReceivedMessage.h"

#include <ProtocolException.hpp>

#include "shared/Log.h"

namespace message
{
    CSmartAckClientMailboxStatusResponseReceivedMessage::CSmartAckClientMailboxStatusResponseReceivedMessage(
        const boost::shared_ptr<const CResponseReceivedMessage>& response)
        : m_status(toMailboxStatus(response->responseData()[0]))
    {
    }

    CSmartAckClientMailboxStatusResponseReceivedMessage::MailboxStatus CSmartAckClientMailboxStatusResponseReceivedMessage::status() const
    {
        return m_status;
    }

    std::string CSmartAckClientMailboxStatusResponseReceivedMessage::toString(MailboxStatus mailboxStatus)
    {
        switch (mailboxStatus)
        {
        case EMPTY:
            return "Empty";
        case FULL:
            return "Full";
        case DOESNT_EXIST:
            return "Doesn't exist";
        default:
            return "";
        }
    }

    CSmartAckClientMailboxStatusResponseReceivedMessage::MailboxStatus CSmartAckClientMailboxStatusResponseReceivedMessage::toMailboxStatus(
        unsigned char byte)
    {
        switch (byte)
        {
        case 0:
            return EMPTY;
        case 1:
            return FULL;
        case 2:
            return DOESNT_EXIST;
        default:
            YADOMS_LOG(error) << boost::format("Smart Ack response : unsupported mailbox status value %1%") % byte;
            return DOESNT_EXIST;
        }
    }
} // namespace message
