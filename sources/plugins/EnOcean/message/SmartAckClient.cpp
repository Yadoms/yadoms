#include "stdafx.h"
#include "SmartAckClient.h"

namespace message
{
    CSmartAckClient::CSmartAckClient(const std::string& id,
                                     const std::string& controllerId,
                                     const unsigned int mailboxIndex)
        : m_id(id),
          m_controllerId(controllerId),
          m_mailboxIndex(mailboxIndex)
    {
    }

    std::string CSmartAckClient::id() const
    {
        return m_id;
    }

    std::string CSmartAckClient::controllerId() const
    {
        return m_controllerId;
    }

    unsigned int CSmartAckClient::mailboxIndex() const
    {
        return m_mailboxIndex;
    }
} // namespace message
