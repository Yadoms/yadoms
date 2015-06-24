#pragma once

//--------------------------------------------------------------
/// \brief	The sequence number provider interface
//--------------------------------------------------------------
class ISequenceNumberProvider
{  
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~ISequenceNumberProvider() {}

   //--------------------------------------------------------------
   /// \brief     Get the next sequence number
   /// \return    Next sequence number
   //--------------------------------------------------------------
   virtual unsigned char getNext() = 0;
};



