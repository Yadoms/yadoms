#include "stdafx.h"
#include "PendingMail.h"

#include <utility>


CPendingMail::CPendingMail(int recipientId,
                           std::string body)
   : m_recipientId(recipientId),
     m_body(std::move(body))
{
}

int CPendingMail::recipientId()
{
   return m_recipientId;
}

std::string CPendingMail::body()
{
   return m_body;
}

bool CPendingMail::retry()
{
   return --m_tries;
}
