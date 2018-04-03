#pragma once

//--------------------------------------------------------------
/// \brief	Class where all Keywords are created and updated
/// \note   
//--------------------------------------------------------------
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
   /// \param[in] GPIONumber          The GPIO Pin to desactivate
   //--------------------------------------------------------------
   virtual void desactivateGPIO(const int GPIONumber) = 0;

private:
};

