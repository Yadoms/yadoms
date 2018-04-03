#pragma once
#include "../IFTD2XManager.h"
#include "../ftdi/FTD2XX.H"

//--------------------------------------------------------------
/// \brief	Class where all Keywords are created and updated
/// \note   
//--------------------------------------------------------------
class CFT2XManager : public IFTD2XManager
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CFT2XManager();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFT2XManager();

   //--------------------------------------------------------------
   /// \brief	    Activate the GPIO pin in parameter
   /// \param[in] GPIONumber          The GPIO Pin to activate
   //--------------------------------------------------------------
   void activateGPIO(const int GPIONumber) override;

   //--------------------------------------------------------------
   /// \brief	    Desactivate the GPIO pin in parameter
   /// \param[in] GPIONumber          The GPIO Pin to desactivate
   //--------------------------------------------------------------
   void desactivateGPIO(const int GPIONumber) override;

private:

   FT_HANDLE ftHandle = NULL;
   HINSTANCE hGetProcIDDLL;

   static const unsigned char m_mask_port1;
   static const unsigned char m_mask_port2;
   static const unsigned char m_mask_no_port;
   static const unsigned char m_enableBitBang;
};