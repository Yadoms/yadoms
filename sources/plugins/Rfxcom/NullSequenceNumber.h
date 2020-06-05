#pragma once

#include "ISequenceNumber.h"

//--------------------------------------------------------------
/// \brief	A always null sequence number
//--------------------------------------------------------------
class CNullSequenceNumber : public ISequenceNumber
{
public:
   CNullSequenceNumber() = default;
   virtual ~CNullSequenceNumber() = default;

   // ISequenceNumber implementation
   void reset(unsigned char value = 0) override
   {
   }

   unsigned char next() override
   {
      return 0;
   }

   unsigned char last() const override
   {
      return 0;
   }

   bool isExpected(unsigned char) const override
   {
      return true;
   }
   // [END] ISequenceNumber implementation
};
