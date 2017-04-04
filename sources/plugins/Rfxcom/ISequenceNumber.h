#pragma once

//--------------------------------------------------------------
/// \brief	The sequence number checker interface
//--------------------------------------------------------------
class ISequenceNumber
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~ISequenceNumber()
   {
   }

   //--------------------------------------------------------------
   /// \brief     Reset the sequence number
   /// \param[in] value New value
   //--------------------------------------------------------------
   virtual void reset(unsigned char value = 0) = 0;

   //--------------------------------------------------------------
   /// \brief     Get the next sequence number (increment counter and returns new value)
   /// \return    Next sequence number
   //--------------------------------------------------------------
   virtual unsigned char next() = 0;

   //--------------------------------------------------------------
   /// \brief     Get the last sequence number (returns current value)
   /// \return    Current sequence number
   //--------------------------------------------------------------
   virtual unsigned char last() const = 0;

   //--------------------------------------------------------------
   /// \brief     Get the last sequence number (returns current value)
   /// \param[in] value Value to check
   /// \return    Current sequence number
   //--------------------------------------------------------------
   virtual bool isExpected(unsigned char value) const = 0;
};
