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
   virtual CNullSequenceNumber() {}

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CNullSequenceNumber() {}

   //--------------------------------------------------------------
   /// \brief     Get the next sequence number
   /// \return    Next sequence number
   //--------------------------------------------------------------
   virtual unsigned char getNext() { return 0; }
};



