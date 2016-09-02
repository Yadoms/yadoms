#pragma once

#include "ISequenceNumberProvider.h"

//--------------------------------------------------------------
/// \brief	A always null sequence number
//--------------------------------------------------------------
class CIncrementSequenceNumber : public ISequenceNumberProvider
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CIncrementSequenceNumber() :m_seqNumber(0) {}

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CIncrementSequenceNumber() {}

   // ISequenceNumberProvider implementation
   virtual void reset() { m_seqNumber = 0; }
   virtual unsigned char next() { return ++m_seqNumber; }
   virtual unsigned char last() const { return m_seqNumber; }
   // [END] ISequenceNumberProvider implementation

private:
   unsigned char m_seqNumber;
};