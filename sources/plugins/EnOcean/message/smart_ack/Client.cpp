#include "stdafx.h"
#include "Client.h"

using namespace message::smart_ack;

CClient::CClient(const std::string& id,
                 const std::string& controllerId,
                 const unsigned int mailboxIndex)
    : m_id(id),
      m_controllerId(controllerId),
      m_mailboxIndex(mailboxIndex)
{
}

std::string CClient::id() const
{
    return m_id;
}

std::string CClient::controllerId() const
{
    return m_controllerId;
}

unsigned int CClient::mailboxIndex() const
{
    return m_mailboxIndex;
}
