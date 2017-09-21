#include "stdafx.h"
#include "ReceiveBufferHandler.h"
#include "rfplayerApi/usb_frame_api.h"

CReceiveBufferHandler::CReceiveBufferHandler(boost::shared_ptr<IMessageHandler> messageHandler)
   : m_messageHandler(messageHandler)
{
}

CReceiveBufferHandler::~CReceiveBufferHandler()
{
}

void CReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   for (unsigned int idx = 0; idx < buffer.size(); ++idx)
      m_content.push_back(buffer[idx]);

   if (isComplete())
   {
      boost::shared_ptr<frames::incoming::CFrame> bufferMessage = popNextMessage();
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

   for (unsigned int i = 0; i < m_content.size()-1; ++i)
   {
      if (m_content[i] == SYNC1_CONTAINER_CONSTANT && m_content[i + 1] == SYNC2_CONTAINER_CONSTANT) //Z and I
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

frames::incoming::EFrameType CReceiveBufferHandler::identifyFrameType()
{
   if (m_content.size() >= 2)
   {
      unsigned char frameType = m_content[2] & 0x70;
      if (frameType == 0x40)
         return frames::incoming::kAsciiFrame;
      if (frameType == 0x00)
         return frames::incoming::kBinaryFrame;
   }
   return frames::incoming::kUnknown;
}


bool CReceiveBufferHandler::isComplete()
{
   if (syncToStartOfFrame())
   {
      switch (identifyFrameType())
      {
         case frames::incoming::kAsciiFrame:
         {
            //search for terminator \0 or \r
            for (unsigned int i = 5; i < m_content.size(); ++i)
            {
               if (m_content[i] == 0x00 || m_content[i] == 0x0D)
                  return true;
            }
            break;
         }
         case frames::incoming::kBinaryFrame:
         {
            unsigned int len = getCurrentBinaryFrameSize();
            return m_content.size() >= (5 + len);
         }
         default:
            break;
      }
   }
   return false;
}

unsigned int CReceiveBufferHandler::getCurrentBinaryFrameSize()
{
   //size is given in bytes 4 and 5
   return (m_content[4] << 8) + m_content[3];
}

boost::shared_ptr<frames::incoming::CFrame> CReceiveBufferHandler::popNextMessage()
{
   if (!isComplete())
      throw shared::exception::CException("CReceiveBufferHandler : Can not pop not completed message. Call isComplete to check if a message is available");

   boost::shared_ptr<frames::incoming::CFrame> result;

   if (syncToStartOfFrame())
   {
      switch (identifyFrameType())
      {
      case frames::incoming::kAsciiFrame:
         {
            //search for terminator \0 or \r
            for (unsigned int i = frames::incoming::CAsciiFrame::HeaderSize; i < m_content.size(); ++i)
            {
               if (m_content[i] == 0x00 || m_content[i] == 0x0D)
               {
                  unsigned int extractedMessageSize = i - frames::incoming::CAsciiFrame::HeaderSize;
                  std::string content;
                  for (size_t idx = frames::incoming::CAsciiFrame::HeaderSize; idx < i; ++idx)
                     content += m_content[idx];

                  result = boost::make_shared<frames::incoming::CFrame>(boost::make_shared<frames::incoming::CAsciiFrame>(m_content[2], m_content[3], m_content[4], content));
                  // Delete extracted data
                  m_content.erase(m_content.begin(), m_content.begin() + extractedMessageSize + frames::incoming::CAsciiFrame::HeaderSize);
               }
            }
            break;
         }
      case frames::incoming::kBinaryFrame:
         {
            //size is given in bytes 4 and 5
            unsigned int len = getCurrentBinaryFrameSize();
            boost::shared_ptr<shared::communication::CByteBuffer> extractedMessage(new shared::communication::CByteBuffer(len));
            for (size_t idx = frames::incoming::CBinaryFrame::HeaderSize; idx < len+ frames::incoming::CBinaryFrame::HeaderSize; ++idx)
               (*extractedMessage)[idx- frames::incoming::CBinaryFrame::HeaderSize] = m_content[idx];

            result = boost::make_shared<frames::incoming::CFrame>(boost::make_shared<frames::incoming::CBinaryFrame>(m_content[2], extractedMessage));

            // Delete extracted data
            m_content.erase(m_content.begin(), m_content.begin() + len + frames::incoming::CBinaryFrame::HeaderSize);
         }
      default:
         break;
      }
   }
   return result;
}

