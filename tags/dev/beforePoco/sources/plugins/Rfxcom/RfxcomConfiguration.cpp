#include "stdafx.h"
#include "RfxcomConfiguration.h"

CRfxcomConfiguration::~CRfxcomConfiguration()
{
}

void CRfxcomConfiguration::initializeWith(const shared::CDataContainer &data)
{
   m_data.initializeWith(data);
}

std::string CRfxcomConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("Serial Port");
}

bool CRfxcomConfiguration::isAEenabled        () const { return m_data.get<bool>("Protocols.content.AE Blyss", false); }
bool CRfxcomConfiguration::isRUBICSONenabled  () const { return m_data.get<bool>("Protocols.content.Rubicson", false); }
bool CRfxcomConfiguration::isFINEOFFSETenabled() const { return m_data.get<bool>("Protocols.content.FineOffset/Viking", false); }
bool CRfxcomConfiguration::isLIGHTING4enabled () const { return m_data.get<bool>("Protocols.content.Lighting4", false); }
bool CRfxcomConfiguration::isRSLenabled       () const { return m_data.get<bool>("Protocols.content.RSL", false); }
bool CRfxcomConfiguration::isSXenabled        () const { return m_data.get<bool>("Protocols.content.Byron SX", false); }
bool CRfxcomConfiguration::isRFU6enabled      () const { return m_data.get<bool>("Protocols.content.RFU", false); }
bool CRfxcomConfiguration::isUNDECODEDenabled () const { return m_data.get<bool>("Protocols.content.undecoded messages", false); }
bool CRfxcomConfiguration::isMERTIKenabled    () const { return m_data.get<bool>("Protocols.content.Mertik", false); }
bool CRfxcomConfiguration::isLWRFenabled      () const { return m_data.get<bool>("Protocols.content.AD LightwaveRF", false); }
bool CRfxcomConfiguration::isHIDEKIenabled    () const { return m_data.get<bool>("Protocols.content.Hideki/UPM", false); }
bool CRfxcomConfiguration::isLACROSSEenabled  () const { return m_data.get<bool>("Protocols.content.La Crosse", false); }
bool CRfxcomConfiguration::isFS20enabled      () const { return m_data.get<bool>("Protocols.content.FS20", false); }
bool CRfxcomConfiguration::isPROGUARDenabled  () const { return m_data.get<bool>("Protocols.content.ProGuard", false); }
bool CRfxcomConfiguration::isBLINDST0enabled  () const { return m_data.get<bool>("Protocols.content.BlindsT0", false); }
bool CRfxcomConfiguration::isBLINDST1enabled  () const { return m_data.get<bool>("Protocols.content.BlindsT1", false); }
bool CRfxcomConfiguration::isX10enabled       () const { return m_data.get<bool>("Protocols.content.X10", false); }
bool CRfxcomConfiguration::isARCenabled       () const { return m_data.get<bool>("Protocols.content.ARC", false); }
bool CRfxcomConfiguration::isACenabled        () const { return m_data.get<bool>("Protocols.content.AC", false); }
bool CRfxcomConfiguration::isHEEUenabled      () const { return m_data.get<bool>("Protocols.content.HomeEasy EU", false); }
bool CRfxcomConfiguration::isMEIANTECHenabled () const { return m_data.get<bool>("Protocols.content.Meiantech", false); }
bool CRfxcomConfiguration::isOREGONenabled    () const { return m_data.get<bool>("Protocols.content.Oregon Scientific", false); }
bool CRfxcomConfiguration::isATIenabled       () const { return m_data.get<bool>("Protocols.content.ATI", false); }
bool CRfxcomConfiguration::isVISONICenabled   () const { return m_data.get<bool>("Protocols.content.Visonic", false); }
