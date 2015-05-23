#include "stdafx.h"
#include <shared/Log.h>
#include "TransceiverStatus.h"
#include <shared/StringExtension.h>


namespace TeleInfoMessages
{

CTransceiverStatus::CTransceiverStatus(const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
{
   //m_firmwareVersion = rbuf.IRESPONSE.msg2;

   // Enabled protocols
   //m_PIREnabled        = rbuf.IRESPONSE.PIREnabled       ;
   //m_MainUnitEnabled   = rbuf.IRESPONSE.MainUnitEnabled  ;
}                         

CTransceiverStatus::~CTransceiverStatus()
{
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CTransceiverStatus::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   BOOST_ASSERT_MSG(false, "Status is a read-only message, can not be encoded");
   throw shared::exception::CInvalidParameter("Status is a read-only message, can not be encoded");
}

void CTransceiverStatus::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   // Nothing to historize
}

const std::string& CTransceiverStatus::getDeviceName() const
{
   return shared::CStringExtension::EmptyString;
} 

void CTransceiverStatus::reset()
{
}

void CTransceiverStatus::traceEnabledProtocols() const
{
   YADOMS_LOG(information) << "TeleInfo configured protocols :";

   //if (m_PIREnabled        ) YADOMS_LOG(information) << "   - PIR Enabled";
   //if (m_MainUnitEnabled  ) YADOMS_LOG(information) << "   - Main Unit Enabled";
}

CTransceiverStatus::EStatusType CTransceiverStatus::getStatusType() const
{
   return m_statusType;
}

unsigned int CTransceiverStatus::getFirmwareVersion() const
{
   return m_firmwareVersion;
}
/* TODO : Faire le changement de configuration
bool CTransceiverStatus::needConfigurationUpdate(const IRfxcomConfiguration& configuration) const
{

   if (
      (configuration.isAEenabled        () == m_AEenabled        ) &&
      (configuration.isRUBICSONenabled  () == m_RUBICSONenabled  ) &&
      (configuration.isFINEOFFSETenabled() == m_FINEOFFSETenabled) &&
      (configuration.isLIGHTING4enabled () == m_LIGHTING4enabled ) &&
      (configuration.isRSLenabled       () == m_RSLenabled       ) &&
      (configuration.isSXenabled        () == m_SXenabled        ) &&
      (configuration.isRFU6enabled      () == m_RFU6enabled      ) &&
      (configuration.isUNDECODEDenabled () == m_UNDECODEDenabled ) &&
      (configuration.isMERTIKenabled    () == m_MERTIKenabled    ) &&
      (configuration.isLWRFenabled      () == m_LWRFenabled      ) &&
      (configuration.isHIDEKIenabled    () == m_HIDEKIenabled    ) &&
      (configuration.isLACROSSEenabled  () == m_LACROSSEenabled  ) &&
      (configuration.isFS20enabled      () == m_FS20enabled      ) &&
      (configuration.isPROGUARDenabled  () == m_PROGUARDenabled  ) &&
      (configuration.isBLINDST0enabled  () == m_BLINDST0enabled  ) &&
      (configuration.isBLINDST1enabled  () == m_BLINDST1enabled  ) &&
      (configuration.isX10enabled       () == m_X10enabled       ) &&
      (configuration.isARCenabled       () == m_ARCenabled       ) &&
      (configuration.isACenabled        () == m_ACenabled        ) &&
      (configuration.isHEEUenabled      () == m_HEEUenabled      ) &&
      (configuration.isMEIANTECHenabled () == m_MEIANTECHenabled ) &&
      (configuration.isOREGONenabled    () == m_OREGONenabled    ) &&
      (configuration.isATIenabled       () == m_ATIenabled       ) &&
      (configuration.isVISONICenabled   () == m_VISONICenabled   )
      )
      return false;

   return true;
}
*/
} // namespace TeleInfoMessages