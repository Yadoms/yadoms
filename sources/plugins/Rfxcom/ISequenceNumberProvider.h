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
   /// \brief     Reset the sequence number
   //--------------------------------------------------------------
   virtual void reset() = 0;

   //--------------------------------------------------------------
   /// \brief     Get the next sequence number
   /// \return    Next sequence number
   //--------------------------------------------------------------
   virtual unsigned char getNext() = 0;
};



