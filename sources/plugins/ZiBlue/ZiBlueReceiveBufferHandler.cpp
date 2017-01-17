#include "stdafx.h"
#include "ZiBlueReceiveBufferHandler.h"
#include <shared/communication/StringBuffer.h>

CZiBlueReceiveBufferHandler::CZiBlueReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler, int receiveBinaryFrameEventId, int receiveAsciiFrameEventId)
   : m_receiveDataEventHandler(receiveDataEventHandler), m_receiveBinaryFrameEventId(receiveBinaryFrameEventId), m_receiveAsciiFrameEventId(receiveAsciiFrameEventId)
{
}

CZiBlueReceiveBufferHandler::~CZiBlueReceiveBufferHandler()
{
}

void CZiBlueReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   for (auto idx = 0; idx < buffer.size(); ++idx)
      m_content.push_back(buffer[idx]);

   if (isComplete())
   {
      CZiBlueReceiveBufferHandler::BufferContainer bufferMessage = popNextMessage();
      notifyEventHandler(bufferMessage);
   }
      
}

void CZiBlueReceiveBufferHandler::flush()
{
   m_content.clear();
}

bool CZiBlueReceiveBufferHandler::syncToStartOfFrame()
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

frames::EFrameType CZiBlueReceiveBufferHandler::identifyFrameType()
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


bool CZiBlueReceiveBufferHandler::isComplete()
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
            //size is given in bytes 4 and 5
            int len = (m_content[3] << 8) + m_content[4];
            return m_content.size() >= (5 + len);
         }
      }
   }
   return false;
}

CZiBlueReceiveBufferHandler::BufferContainer CZiBlueReceiveBufferHandler::popNextMessage()
{
   if (!isComplete())
      throw shared::exception::CException("CZiBlueReceiveBufferHandler : Can not pop not completed message. Call isComplete to check if a message is available");

   BufferContainer container;

   if (syncToStartOfFrame())
   {
      container.frameType = identifyFrameType();
      switch (container.frameType)
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

                  container.asciiBuffer = boost::make_shared<frames::CAsciiFrame>(m_content[2], m_content[3], m_content[4], content);
                  // Delete extracted data
                  m_content.erase(m_content.begin(), m_content.begin() + extractedMessageSize + frames::CAsciiFrame::HeaderSize);
               }
            }
            break;
         }
      case frames::kBinaryFrame:
         {
            //size is given in bytes 4 and 5
            int len = (m_content[3] << 8) + m_content[4];
            boost::shared_ptr<shared::communication::CByteBuffer> extractedMessage(new shared::communication::CByteBuffer(len));
            for (size_t idx = frames::CBinaryFrame::HeaderSize; idx < len; ++idx)
               (*extractedMessage)[idx] = m_content[idx];
            container.binaryBuffer = boost::make_shared<frames::CBinaryFrame>();
            container.binaryBuffer->m_content = extractedMessage;
            // Delete extracted data
            m_content.erase(m_content.begin(), m_content.begin() + len + frames::CBinaryFrame::HeaderSize);
         }
      }
   }
   return container;
}

void CZiBlueReceiveBufferHandler::notifyEventHandler(BufferContainer & bufferContainer) const
{
   switch (bufferContainer.frameType)
   {
   case frames::kBinaryFrame:
      m_receiveDataEventHandler.postEvent< boost::shared_ptr<frames::CBinaryFrame> >(m_receiveBinaryFrameEventId, bufferContainer.binaryBuffer);
      break;
   case frames::kAsciiFrame:
      m_receiveDataEventHandler.postEvent< boost::shared_ptr<frames::CAsciiFrame> >(m_receiveAsciiFrameEventId, bufferContainer.asciiBuffer);
      break;
   }
   
      
}

