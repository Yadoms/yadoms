#include "stdafx.h"
#include "RfxComReceiveBuffer.h"


CRfxcommReceiveBuffer::CRfxcommReceiveBuffer()
{
}

CRfxcommReceiveBuffer::~CRfxcommReceiveBuffer()
{
}

void CRfxcommReceiveBuffer::flush()
{
   m_content.clear();
}

bool CRfxcommReceiveBuffer::isComplete() const
{
   if (m_content.empty())
      return false;

   // The message size is provided in the first byte of the message
   // (see RFXCom specifications). This value counts all bytes except itself.
   // So a message is considered complete if its size is at least the value indicated
   // in the first byte + 1.
   if (m_content.size() < ((size_t)m_content[0] + 1))
      return false;

   // A message is complete
   return true;
}

bool CRfxcommReceiveBuffer::isEmpty() const
{
   return m_content.empty();
}

void CRfxcommReceiveBuffer::append(const CByteBuffer& toAppend)
{
   for (size_t idx = 0 ; idx < toAppend.size() ; ++ idx)
      m_content.push_back(toAppend.content()[idx]);
}

boost::shared_ptr<CByteBuffer> CRfxcommReceiveBuffer::popNextMessage()
{
   BOOST_ASSERT_MSG(isComplete(), "CRfxcommReceiveBuffer : Can not pop not completed message. Call isComplete to check if a message is available");

   // The message size is provided in the first byte of the message
   // (see RFXCom specifications). This value counts all bytes except itself.
   // So the message size is this value + 1.
   const size_t extractedMessageSize = m_content[0] + 1;
   boost::shared_ptr<unsigned char[]> extractedMessage(new unsigned char[extractedMessageSize]);
   for (size_t idx = 0 ; idx < extractedMessageSize ; ++ idx)
      extractedMessage[idx] = m_content[idx];

   boost::shared_ptr<CByteBuffer> nextMessage(new CByteBuffer(extractedMessage.get(), extractedMessageSize));

   // Delete extracted data
   m_content.erase(m_content.begin(), m_content.begin() + extractedMessageSize);

   return nextMessage;
}