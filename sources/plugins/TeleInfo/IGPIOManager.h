#pragma once

//--------------------------------------------------------------
/// \brief	The sequence number checker interface
//--------------------------------------------------------------
class IGPIOManager
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IGPIOManager()
   {
   }

   //--------------------------------------------------------------
   /// \brief     Reset the sequence number
   /// \param[in] value New value
   //--------------------------------------------------------------
   virtual void front() = 0;

   //--------------------------------------------------------------
   /// \brief     Get the next sequence number (increment counter and returns new value)
   /// \return    Next sequence number
   //--------------------------------------------------------------
   virtual void next() = 0;

   //--------------------------------------------------------------
   /// \brief     Get the next sequence number (increment counter and returns new value)
   /// \return    Next sequence number
   //--------------------------------------------------------------
   virtual int getGPIO() = 0;

   //--------------------------------------------------------------
   /// \brief     Get the last sequence number (returns current value)
   /// \param[in] value Value to check
   /// \return    Current sequence number
   //--------------------------------------------------------------
   virtual bool isLast() const = 0;
};
