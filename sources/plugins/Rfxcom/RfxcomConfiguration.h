#pragma once

#include "IRfxcomConfiguration.h"
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

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
   void initializeWith(const shared::CDataContainer &data) override;
   bool comIsEthernet() const override;
   std::string getEthernetAddress() const override;
   std::string getEthernetPort() const override;
   std::string getSerialPort() const override;
   CPairingHelper::EPairingMode getPairingMode() const override;
   bool isAEenabled() const override;
   bool isRUBICSONenabled() const override;
   bool isFINEOFFSETenabled() const override;
   bool isLIGHTING4enabled() const override;
   bool isRSLenabled() const override;
   bool isSXenabled() const override;
   bool isIMAGINTRONIXenabled() const override;
   bool isUNDECODEDenabled() const override;
   bool isMERTIKenabled() const override;
   bool isLWRFenabled() const override;
   bool isHIDEKIenabled() const override;
   bool isLACROSSEenabled() const override;
   bool isLEGRANDenabled() const override;
   bool isBLINDST0enabled() const override;
   bool isBLINDST1enabled() const override;
   bool isX10enabled() const override;
   bool isARCenabled() const override;
   bool isACenabled() const override;
   bool isHEEUenabled() const override;
   bool isMEIANTECHenabled() const override;
   bool isOREGONenabled() const override;
   bool isATIenabled() const override;
   bool isVISONICenabled() const override;
   bool isKeeLoqenabled() const override;
   bool isHomeConfortenabled() const override;
   // [END] IRfxcomConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   yApi::YPluginConfiguration m_configuration;
};

