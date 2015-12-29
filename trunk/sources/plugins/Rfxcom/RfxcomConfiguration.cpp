#include "stdafx.h"
#include "RfxcomConfiguration.h"
#include <shared/StringExtension.h>

CRfxcomConfiguration::~CRfxcomConfiguration()
{
}

void CRfxcomConfiguration::initializeWith(const shared::CDataContainer &data)
{
   m_data.initializeWith(data);
}

bool CRfxcomConfiguration::comIsEthernet() const
{
   return false;
}

std::string CRfxcomConfiguration::getEthernetAddress() const
{
   return shared::CStringExtension::EmptyString;
}

std::string CRfxcomConfiguration::getEthernetPort() const
{
   return shared::CStringExtension::EmptyString;
}

std::string CRfxcomConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("SerialPort");
}

unsigned int CRfxcomConfiguration::getSendRepetitions() const
{
   return m_data.getWithDefault<unsigned int>("SendRepetitions", 0);
}

bool CRfxcomConfiguration::isAEenabled          () const { return m_data.getWithDefault<bool>("Protocols.content.AE Blyss", false); }
bool CRfxcomConfiguration::isRUBICSONenabled    () const { return m_data.getWithDefault<bool>("Protocols.content.Rubicson", false); }
bool CRfxcomConfiguration::isFINEOFFSETenabled  () const { return m_data.getWithDefault<bool>("Protocols.content.FineOffset/Viking", false); }
bool CRfxcomConfiguration::isLIGHTING4enabled   () const { return m_data.getWithDefault<bool>("Protocols.content.Lighting4", false); }
bool CRfxcomConfiguration::isRSLenabled         () const { return m_data.getWithDefault<bool>("Protocols.content.RSL", false); }
bool CRfxcomConfiguration::isSXenabled          () const { return m_data.getWithDefault<bool>("Protocols.content.Byron SX", false); }
bool CRfxcomConfiguration::isIMAGINTRONIXenabled() const { return m_data.getWithDefault<bool>("Protocols.content.Imagintronix", false); }
bool CRfxcomConfiguration::isUNDECODEDenabled   () const { return m_data.getWithDefault<bool>("Protocols.content.undecoded messages", false); }
bool CRfxcomConfiguration::isMERTIKenabled      () const { return m_data.getWithDefault<bool>("Protocols.content.Mertik", false); }
bool CRfxcomConfiguration::isLWRFenabled        () const { return m_data.getWithDefault<bool>("Protocols.content.AD LightwaveRF", false); }
bool CRfxcomConfiguration::isHIDEKIenabled      () const { return m_data.getWithDefault<bool>("Protocols.content.Hideki/UPM", false); }
bool CRfxcomConfiguration::isLACROSSEenabled    () const { return m_data.getWithDefault<bool>("Protocols.content.La Crosse", false); }
bool CRfxcomConfiguration::isFS20enabled        () const { return m_data.getWithDefault<bool>("Protocols.content.FS20", false); }
bool CRfxcomConfiguration::isPROGUARDenabled    () const { return m_data.getWithDefault<bool>("Protocols.content.ProGuard", false); }
bool CRfxcomConfiguration::isBLINDST0enabled    () const { return m_data.getWithDefault<bool>("Protocols.content.BlindsT0", false); }
bool CRfxcomConfiguration::isBLINDST1enabled    () const { return m_data.getWithDefault<bool>("Protocols.content.BlindsT1", false); }
bool CRfxcomConfiguration::isX10enabled         () const { return m_data.getWithDefault<bool>("Protocols.content.X10", false); }
bool CRfxcomConfiguration::isARCenabled         () const { return m_data.getWithDefault<bool>("Protocols.content.ARC", false); }
bool CRfxcomConfiguration::isACenabled          () const { return m_data.getWithDefault<bool>("Protocols.content.AC", false); }
bool CRfxcomConfiguration::isHEEUenabled        () const { return m_data.getWithDefault<bool>("Protocols.content.HomeEasy EU", false); }
bool CRfxcomConfiguration::isMEIANTECHenabled   () const { return m_data.getWithDefault<bool>("Protocols.content.Meiantech", false); }
bool CRfxcomConfiguration::isOREGONenabled      () const { return m_data.getWithDefault<bool>("Protocols.content.Oregon Scientific", false); }
bool CRfxcomConfiguration::isATIenabled         () const { return m_data.getWithDefault<bool>("Protocols.content.ATI", false); }
bool CRfxcomConfiguration::isVISONICenabled     () const { return m_data.getWithDefault<bool>("Protocols.content.Visonic", false); }
bool CRfxcomConfiguration::isKeeLoqenabled      () const { return m_data.getWithDefault<bool>("Protocols.content.KeeLoq", false); }
bool CRfxcomConfiguration::isHomeConfortenabled () const { return m_data.getWithDefault<bool>("Protocols.content.HomeConfort", false); }
