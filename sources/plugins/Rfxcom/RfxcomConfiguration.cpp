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

bool CRfxcomConfiguration::isAEenabled        () const { return m_data.get<bool>("Protocols.values.AE Blyss", false); }
bool CRfxcomConfiguration::isRUBICSONenabled  () const { return m_data.get<bool>("Protocols.values.Rubicson", false); }
bool CRfxcomConfiguration::isFINEOFFSETenabled() const { return m_data.get<bool>("Protocols.values.FineOffset/Viking", false); }
bool CRfxcomConfiguration::isLIGHTING4enabled () const { return m_data.get<bool>("Protocols.values.Lighting4", false); }
bool CRfxcomConfiguration::isRSLenabled       () const { return m_data.get<bool>("Protocols.values.RSL", false); }
bool CRfxcomConfiguration::isSXenabled        () const { return m_data.get<bool>("Protocols.values.Byron SX", false); }
bool CRfxcomConfiguration::isRFU6enabled      () const { return m_data.get<bool>("Protocols.values.RFU", false); }
bool CRfxcomConfiguration::isUNDECODEDenabled () const { return m_data.get<bool>("Protocols.values.undecoded messages", false); }
bool CRfxcomConfiguration::isMERTIKenabled    () const { return m_data.get<bool>("Protocols.values.Mertik", false); }
bool CRfxcomConfiguration::isLWRFenabled      () const { return m_data.get<bool>("Protocols.values.AD LightwaveRF", false); }
bool CRfxcomConfiguration::isHIDEKIenabled    () const { return m_data.get<bool>("Protocols.values.Hideki/UPM", false); }
bool CRfxcomConfiguration::isLACROSSEenabled  () const { return m_data.get<bool>("Protocols.values.La Crosse", false); }
bool CRfxcomConfiguration::isFS20enabled      () const { return m_data.get<bool>("Protocols.values.FS20", false); }
bool CRfxcomConfiguration::isPROGUARDenabled  () const { return m_data.get<bool>("Protocols.values.ProGuard", false); }
bool CRfxcomConfiguration::isBLINDST0enabled  () const { return m_data.get<bool>("Protocols.values.BlindsT0", false); }
bool CRfxcomConfiguration::isBLINDST1enabled  () const { return m_data.get<bool>("Protocols.values.BlindsT1", false); }
bool CRfxcomConfiguration::isX10enabled       () const { return m_data.get<bool>("Protocols.values.X10", false); }
bool CRfxcomConfiguration::isARCenabled       () const { return m_data.get<bool>("Protocols.values.ARC", false); }
bool CRfxcomConfiguration::isACenabled        () const { return m_data.get<bool>("Protocols.values.AC", false); }
bool CRfxcomConfiguration::isHEEUenabled      () const { return m_data.get<bool>("Protocols.values.HomeEasy EU", false); }
bool CRfxcomConfiguration::isMEIANTECHenabled () const { return m_data.get<bool>("Protocols.values.Meiantech", false); }
bool CRfxcomConfiguration::isOREGONenabled    () const { return m_data.get<bool>("Protocols.values.Oregon Scientific", false); }
bool CRfxcomConfiguration::isATIenabled       () const { return m_data.get<bool>("Protocols.values.ATI", false); }
bool CRfxcomConfiguration::isVISONICenabled   () const { return m_data.get<bool>("Protocols.values.Visonic", false); }
