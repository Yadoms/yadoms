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
   CIncrementSequenceNumber()
      : m_seqNumber(0)
   {
   }

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CIncrementSequenceNumber()
   {
   }

   // ISequenceNumberProvider implementation
   void reset() override
   {
      m_seqNumber = 0;
   }

   unsigned char next() override
   {
      return ++ m_seqNumber;
   }

   unsigned char last() const override
   {
      return m_seqNumber;
   }

   // [END] ISequenceNumberProvider implementation

private:
   unsigned char m_seqNumber;
};

