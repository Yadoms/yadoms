#pragma once

#include "ISequenceNumber.h"

//--------------------------------------------------------------
/// \brief	A always null sequence number
//--------------------------------------------------------------
class CIncrementSequenceNumberQueued : public ISequenceNumber
{
public:
   CIncrementSequenceNumberQueued():
      m_backSeqNumber(0),
      m_frontSeqNumber(0)
   {
   }

   virtual ~CIncrementSequenceNumberQueued() = default;

   // ISequenceNumber implementation
   void reset(unsigned char value = 0) override
   {
      m_backSeqNumber = value;
      m_frontSeqNumber = value;
   }

   unsigned char next() override
   {
      return ++m_frontSeqNumber;
   }

   unsigned char last() const override
   {
      return m_frontSeqNumber;
   }

   bool isExpected(unsigned char value) override
   {
      return value != ++m_backSeqNumber;
   }

   // [END] ISequenceNumber implementation
private:
   unsigned char m_backSeqNumber;
   unsigned char m_frontSeqNumber;
};
