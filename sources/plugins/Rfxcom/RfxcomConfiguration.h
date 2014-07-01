#pragma once

#include "IRfxcomConfiguration.h"


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CRfxcomConfiguration : public IRfxcomConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CRfxcomConfiguration();

   // IRfxcomConfiguration implementation
   virtual void initializeWith(const shared::CDataContainer &data);
   virtual std::string getSerialPort() const;
   virtual bool isAEenabled        () const;
   virtual bool isRUBICSONenabled  () const;
   virtual bool isFINEOFFSETenabled() const;
   virtual bool isLIGHTING4enabled () const;
   virtual bool isRSLenabled       () const;
   virtual bool isSXenabled        () const;
   virtual bool isRFU6enabled      () const;
   virtual bool isUNDECODEDenabled () const;
   virtual bool isMERTIKenabled    () const;
   virtual bool isLWRFenabled      () const;
   virtual bool isHIDEKIenabled    () const;
   virtual bool isLACROSSEenabled  () const;
   virtual bool isFS20enabled      () const;
   virtual bool isPROGUARDenabled  () const;
   virtual bool isBLINDST0enabled  () const;
   virtual bool isBLINDST1enabled  () const;
   virtual bool isX10enabled       () const;
   virtual bool isARCenabled       () const;
   virtual bool isACenabled        () const;
   virtual bool isHEEUenabled      () const;
   virtual bool isMEIANTECHenabled () const;
   virtual bool isOREGONenabled    () const;
   virtual bool isATIenabled       () const;
   virtual bool isVISONICenabled   () const;
   // [END] IRfxcomConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};

