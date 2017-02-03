#include "stdafx.h"
#include "ReceiveBufferHandler.h"
#include <shared/communication/StringBuffer.h>

CReceiveBufferHandler::CReceiveBufferHandler(boost::shared_ptr<IMessageHandler> messageHandler)
   : m_messageHandler(messageHandler)
{
}

CReceiveBufferHandler::~CReceiveBufferHandler()
{
}

void CReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   for (auto idx = 0; idx < buffer.size(); ++idx)
      m_content.push_back(buffer[idx]);

   if (isComplete())
   {
      boost::shared_ptr<frames::CFrame> bufferMessage = popNextMessage();
      if (bufferMessage)
      {
         m_messageHandler->onReceived(bufferMessage);
      }
   }
      
}

void CReceiveBufferHandler::flush()
{
   m_content.clear();
}

bool CReceiveBufferHandler::syncToStartOfFrame()
{
   // The header is 5 bytes minimum
   if (m_content.size() < 5)
      return false;

   for (int i = 0; i < m_content.size()-1; ++i)
   {
      if (m_content[i] == 0x5A && m_content[i + 1] == 0x49) //Z and I
      {
         //ZI found,
         //if the buffer contains any data before ZI, remove it
         if (i != 0)
         {
            m_content.erase(m_content.begin(), m_content.begin() + i);
         }
         return true;
      }
   }
   return false;
}

frames::EFrameType CReceiveBufferHandler::identifyFrameType()
{
   if (m_content.size() >= 2)
   {
      unsigned char frameType = m_content[2] & 0x70;
      if (frameType == 0x40)
         return frames::kAsciiFrame;
      if (frameType == 0x00)
         return frames::kBinaryFrame;
   }
   return frames::kUnknown;
}


bool CReceiveBufferHandler::isComplete()
{
   if (syncToStartOfFrame())
   {
      switch (identifyFrameType())
      {
         case frames::kAsciiFrame:
         {
            //search for terminator \0 or \r
            for (int i = 5; i < m_content.size(); ++i)
            {
               if (m_content[i] == 0x00 || m_content[i] == 0x0D)
                  return true;
            }
            break;
         }
         case frames::kBinaryFrame:
         {
            int len = getCurrentBinaryFrameSize();
            return m_content.size() >= (5 + len);
         }
      }
   }
   return false;
}

const int CReceiveBufferHandler::getCurrentBinaryFrameSize()
{
   //size is given in bytes 4 and 5
   return (m_content[4] << 8) + m_content[3];
}

boost::shared_ptr<frames::CFrame> CReceiveBufferHandler::popNextMessage()
{
   if (!isComplete())
      throw shared::exception::CException("CReceiveBufferHandler : Can not pop not completed message. Call isComplete to check if a message is available");

   boost::shared_ptr<frames::CFrame> result;

   if (syncToStartOfFrame())
   {
      switch (identifyFrameType())
      {
      case frames::kAsciiFrame:
         {
            //search for terminator \0 or \r
            for (int i = frames::CAsciiFrame::HeaderSize; i < m_content.size(); ++i)
            {
               if (m_content[i] == 0x00 || m_content[i] == 0x0D)
               {
                  int extractedMessageSize = i - frames::CAsciiFrame::HeaderSize;
                  std::string content;
                  for (size_t idx = frames::CAsciiFrame::HeaderSize; idx < i; ++idx)
                     content += m_content[idx];

                  result = boost::make_shared<frames::CFrame>(boost::make_shared<frames::CAsciiFrame>(m_content[2], m_content[3], m_content[4], content));
                  // Delete extracted data
                  m_content.erase(m_content.begin(), m_content.begin() + extractedMessageSize + frames::CAsciiFrame::HeaderSize);
               }
            }
            break;
         }
      case frames::kBinaryFrame:
         {
            //size is given in bytes 4 and 5
            int len = getCurrentBinaryFrameSize();
            boost::shared_ptr<shared::communication::CByteBuffer> extractedMessage(new shared::communication::CByteBuffer(len));
            for (size_t idx = frames::CBinaryFrame::HeaderSize; idx < len+ frames::CBinaryFrame::HeaderSize; ++idx)
               (*extractedMessage)[idx- frames::CBinaryFrame::HeaderSize] = m_content[idx];

            result = boost::make_shared<frames::CFrame>(boost::make_shared<frames::CBinaryFrame>(m_content[2], extractedMessage));

            // Delete extracted data
            m_content.erase(m_content.begin(), m_content.begin() + len + frames::CBinaryFrame::HeaderSize);
         }
      }
   }
   return result;
}

