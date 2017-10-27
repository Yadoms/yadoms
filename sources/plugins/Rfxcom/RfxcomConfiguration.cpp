#include "stdafx.h"
#include "RfxcomConfiguration.h"

CRfxcomConfiguration::~CRfxcomConfiguration()
{
}

void CRfxcomConfiguration::initializeWith(const shared::CDataContainer &data)
{
   m_configuration.initializeWith(data);
}

bool CRfxcomConfiguration::comIsEthernet() const
{
   return false;
}

std::string CRfxcomConfiguration::getEthernetAddress() const
{
   return std::string();
}

std::string CRfxcomConfiguration::getEthernetPort() const
{
   return std::string();
}

std::string CRfxcomConfiguration::getSerialPort() const
{
   return m_configuration.get<std::string>("SerialPort");
}

bool CRfxcomConfiguration::isAEenabled() const { return m_configuration.get<bool>("Protocols.content.AE Blyss"); }
bool CRfxcomConfiguration::isRUBICSONenabled() const { return m_configuration.get<bool>("Protocols.content.Rubicson"); }
bool CRfxcomConfiguration::isFINEOFFSETenabled() const { return m_configuration.get<bool>("Protocols.content.FineOffset/Viking"); }
bool CRfxcomConfiguration::isLIGHTING4enabled() const { return m_configuration.get<bool>("Protocols.content.Lighting4"); }
bool CRfxcomConfiguration::isRSLenabled() const { return m_configuration.get<bool>("Protocols.content.RSL"); }
bool CRfxcomConfiguration::isSXenabled() const { return m_configuration.get<bool>("Protocols.content.Byron SX"); }
bool CRfxcomConfiguration::isIMAGINTRONIXenabled() const { return m_configuration.get<bool>("Protocols.content.Imagintronix"); }
bool CRfxcomConfiguration::isUNDECODEDenabled() const { return m_configuration.get<bool>("Protocols.content.undecoded messages"); }
bool CRfxcomConfiguration::isMERTIKenabled() const { return m_configuration.get<bool>("Protocols.content.Mertik"); }
bool CRfxcomConfiguration::isLWRFenabled() const { return m_configuration.get<bool>("Protocols.content.AD LightwaveRF"); }
bool CRfxcomConfiguration::isHIDEKIenabled() const { return m_configuration.get<bool>("Protocols.content.Hideki/UPM"); }
bool CRfxcomConfiguration::isLACROSSEenabled() const { return m_configuration.get<bool>("Protocols.content.La Crosse"); }
bool CRfxcomConfiguration::isLEGRANDenabled() const { return m_configuration.get<bool>("Protocols.content.FS20"); }
bool CRfxcomConfiguration::isBLINDST0enabled() const { return m_configuration.get<bool>("Protocols.content.BlindsT0"); }
bool CRfxcomConfiguration::isBLINDST1enabled() const { return m_configuration.get<bool>("Protocols.content.BlindsT1"); }
bool CRfxcomConfiguration::isX10enabled() const { return m_configuration.get<bool>("Protocols.content.X10"); }
bool CRfxcomConfiguration::isARCenabled() const { return m_configuration.get<bool>("Protocols.content.ARC"); }
bool CRfxcomConfiguration::isACenabled() const { return m_configuration.get<bool>("Protocols.content.AC"); }
bool CRfxcomConfiguration::isHEEUenabled() const { return m_configuration.get<bool>("Protocols.content.HomeEasy EU"); }
bool CRfxcomConfiguration::isMEIANTECHenabled() const { return m_configuration.get<bool>("Protocols.content.Meiantech"); }
bool CRfxcomConfiguration::isOREGONenabled() const { return m_configuration.get<bool>("Protocols.content.Oregon Scientific"); }
bool CRfxcomConfiguration::isATIenabled() const { return m_configuration.get<bool>("Protocols.content.ATI"); }
bool CRfxcomConfiguration::isVISONICenabled() const { return m_configuration.get<bool>("Protocols.content.Visonic"); }
bool CRfxcomConfiguration::isKeeLoqenabled() const { return m_configuration.get<bool>("Protocols.content.KeeLoq"); }
bool CRfxcomConfiguration::isHomeConfortenabled() const { return m_configuration.get<bool>("Protocols.content.HomeConfort"); }
