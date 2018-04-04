#pragma once
#include "../IFTD2XManager.h"
#include "../ftdi/FTD2XX.H"

//--------------------------------------------------------------
/// \brief	Class to access specific fonctions to the FT2X driver
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

   // IFTD2XManager implementation
   void activateGPIO(const int GPIONumber) override;
   void desactivateGPIO() override;
   // [END] IFTD2XManager implementation

private:

   FT_HANDLE ftHandle = NULL;
   HINSTANCE hGetProcIDDLL;

   static const unsigned char m_mask_port1;
   static const unsigned char m_mask_port2;
   static const unsigned char m_mask_no_port;
   static const unsigned char m_enableBitBang;
};