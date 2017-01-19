#include "stdafx.h"
#include "Frame.h"
#include <shared/communication/BufferLogger.h>

namespace frames {


   CFrame::CFrame(boost::shared_ptr<CBinaryFrame> binaryBuffer)
      :m_frameType(kBinaryFrame), m_binaryBuffer(binaryBuffer)
   {
   }

   CFrame::CFrame(boost::shared_ptr<CAsciiFrame> asciiBuffer)
      : m_frameType(kAsciiFrame), m_asciiBuffer(asciiBuffer)
   {
   }

   CFrame::~CFrame()
   {
   }

   bool CFrame::isBinary() const
   {
      return m_frameType == kBinaryFrame;
   }

   bool CFrame::isAscii() const
   {
      return m_frameType == kAsciiFrame;
   }

   boost::shared_ptr<CBinaryFrame> CFrame::getBinary() const
   {
      return m_binaryBuffer;
   }

   boost::shared_ptr<CAsciiFrame> CFrame::getAscii() const
   {
      return m_asciiBuffer;
   }

   const std::string CFrame::toString() const
   {
      switch (m_frameType)
      {
      case kAsciiFrame:
         return m_asciiBuffer->getContent();

      case kBinaryFrame:
         return shared::communication::CBufferLogger::byteBufferToHexString(*m_binaryBuffer->m_content);
      }
      return "";
   }
} //namespace frames



