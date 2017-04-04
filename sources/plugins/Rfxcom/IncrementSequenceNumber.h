#pragma once

#include "ISequenceNumber.h"

//--------------------------------------------------------------
/// \brief	A always null sequence number
//--------------------------------------------------------------
class CIncrementSequenceNumber : public ISequenceNumber
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CIncrementSequenceNumber() : m_seqNumber(0)
   {
   }

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CIncrementSequenceNumber()
   {
   }

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
