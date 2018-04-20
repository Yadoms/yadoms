#pragma once

//-----------------------------------------------------------------
/// \brief	Interface to the FT2X specific function access.
/// \note   
//-----------------------------------------------------------------
class IFTD2XManager
{
public:

   //--------------------------------------------------------------
   /// \brief	    Activate the GPIO pin in parameter
   /// \param[in] GPIONumber          The GPIO Pin to activate
   //--------------------------------------------------------------
   virtual void activateGPIO(const int GPIONumber) = 0;

   //--------------------------------------------------------------
   /// \brief	    Desactivate the GPIO pin in parameter
   //--------------------------------------------------------------
   virtual void desactivateGPIO() = 0;

private:
};