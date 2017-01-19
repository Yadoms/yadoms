#pragma once
#include "AsciiFrame.h"
#include "BinaryFrame.h"
#include "FrameType.h"

namespace frames {

   
   class CFrame
   {
   public:
      CFrame(boost::shared_ptr<CBinaryFrame> binaryBuffer);
      CFrame(boost::shared_ptr<CAsciiFrame> asciiBuffer);   
      virtual ~CFrame();
      bool isBinary() const;
      bool isAscii() const;
      boost::shared_ptr<CBinaryFrame> getBinary() const;
      boost::shared_ptr<CAsciiFrame> getAscii() const;
      const std::string toString() const;
   private:
      EFrameType m_frameType;
      boost::shared_ptr<CBinaryFrame> m_binaryBuffer;
      boost::shared_ptr<CAsciiFrame> m_asciiBuffer;
   };
   
} //namespace frames



