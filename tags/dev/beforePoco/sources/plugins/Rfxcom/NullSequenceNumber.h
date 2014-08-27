#pragma once

#include "ISequenceNumberProvider.h"

//--------------------------------------------------------------
/// \brief	A always null sequence number
//--------------------------------------------------------------
class CNullSequenceNumber : public ISequenceNumberProvider
{  
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CNullSequenceNumber() {}

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CNullSequenceNumber() {}

   // ISequenceNumberProvider implementation
   virtual void reset() {}
   virtual unsigned char next() { return 0; }
   virtual unsigned char last() { return 0; }
   // [END] ISequenceNumberProvider implementation
};



