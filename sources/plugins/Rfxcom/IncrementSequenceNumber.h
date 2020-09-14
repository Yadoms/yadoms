#pragma once

#include "ISequenceNumber.h"

//--------------------------------------------------------------
/// \brief	A always null sequence number
//--------------------------------------------------------------
class CIncrementSequenceNumber : public ISequenceNumber
{
public:
   CIncrementSequenceNumber() : m_seqNumber(0)
   {
   }

   virtual ~CIncrementSequenceNumber() = default;

   // ISequenceNumber implementation
   void reset(unsigned char value = 0) override
   {
      m_seqNumber = value;
   }

   unsigned char next() override
   {
      return ++m_seqNumber;
   }

   unsigned char last() const override
   {
      return m_seqNumber;
   }

   bool isExpected(unsigned char value) const override
   {
      return value == m_seqNumber;
   }

   // [END] ISequenceNumber implementation
private:
   unsigned char m_seqNumber;
};
