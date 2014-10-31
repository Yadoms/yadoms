#include "stdafx.h"
#include "Transceiver.h"
#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/exception/InvalidParameter.hpp>
#include "RfxcomFactory.h"
#include "rfxcomMessages/Ack.h"
#include "rfxcomMessages/Barometric.h"
#include "rfxcomMessages/Chime.h"
#include "rfxcomMessages/Current.h"
#include "rfxcomMessages/CurrentEnergy.h"
#include "rfxcomMessages/Curtain1.h"
#include "rfxcomMessages/Fan.h"
#include "rfxcomMessages/Energy.h"
#include "rfxcomMessages/FS20.h"
#include "rfxcomMessages/Humidity.h"
#include "rfxcomMessages/Lighting1.h"
#include "rfxcomMessages/Lighting2.h"
#include "rfxcomMessages/Lighting3.h"
#include "rfxcomMessages/Lighting4.h"
#include "rfxcomMessages/Lighting6.h"
#include "rfxcomMessages/Power.h"
#include "rfxcomMessages/Rain.h"
#include "rfxcomMessages/RFXMeter.h"
#include "rfxcomMessages/RFXSensor.h"
#include "rfxcomMessages/Rfy.h"
#include "rfxcomMessages/Temp.h"
#include "rfxcomMessages/TempHumidity.h"
#include "rfxcomMessages/TempHumidityBarometric.h"
#include "rfxcomMessages/TransceiverStatus.h"
#include "rfxcomMessages/UV.h"
#include "rfxcomMessages/Weight.h"
#include "rfxcomMessages/Wind.h"
#include "IncrementSequenceNumber.h"
#include <shared/communication/PortException.hpp>
#include "ProtocolException.hpp"


CTransceiver::CTransceiver()
   :m_seqNumberProvider(new CIncrementSequenceNumber())
{
}

CTransceiver::~CTransceiver()
{
}

const shared::communication::CByteBuffer CTransceiver::buildResetCmd() const
{
   // Raz sequence number
   m_seqNumberProvider->reset();

   RBUF request;
   MEMCLEAR(request.ICMND);   // For better performance, just clear the needed sub-structure of RBUF

   request.ICMND.packetlength = ENCODE_PACKET_LENGTH(ICMND);
   request.ICMND.packettype = pTypeInterfaceControl;
   request.ICMND.subtype = sTypeInterfaceCommand;
   request.ICMND.seqnbr = m_seqNumberProvider->last();
   request.ICMND.cmnd = cmdRESET;

   return shared::communication::CByteBuffer((BYTE*)&request.ICMND, sizeof(request.ICMND));
}

const shared::communication::CByteBuffer CTransceiver::buildGetStatusCmd() const
{
   RBUF request;
   MEMCLEAR(request.ICMND);   // For better performance, just clear the needed sub-structure of RBUF

   request.ICMND.packetlength = ENCODE_PACKET_LENGTH(ICMND);
   request.ICMND.packettype = pTypeInterfaceControl;
   request.ICMND.subtype = sTypeInterfaceCommand;
   request.ICMND.seqnbr = m_seqNumberProvider->next();
   request.ICMND.cmnd = cmdSTATUS;

   return shared::communication::CByteBuffer((BYTE*)&request.ICMND, sizeof(request.ICMND));
}

const shared::communication::CByteBuffer CTransceiver::buildSetModeCmd(unsigned char frequency, const IRfxcomConfiguration& configuration) const
{
   RBUF request;
   MEMCLEAR(request.ICMND);   // For better performance, just clear the needed sub-structure of RBUF

   request.ICMND.packetlength = ENCODE_PACKET_LENGTH(ICMND);
   request.ICMND.packettype = pTypeInterfaceControl;
   request.ICMND.subtype = sTypeInterfaceCommand;
   request.ICMND.seqnbr = m_seqNumberProvider->next();
   request.ICMND.cmnd = cmdSETMODE;

   // Frequency
   request.ICMND.msg1 = frequency;

   // Add protocols activation
   request.ICMND.msg3 = 0;
   if (configuration.isUNDECODEDenabled() ) request.ICMND.msg3 |= 0x80;
   if (configuration.isRFU6enabled()      ) request.ICMND.msg3 |= 0x40;
   if (configuration.isSXenabled()        ) request.ICMND.msg3 |= 0x20;
   if (configuration.isRSLenabled()       ) request.ICMND.msg3 |= 0x10;
   if (configuration.isLIGHTING4enabled() ) request.ICMND.msg3 |= 0x08;
   if (configuration.isFINEOFFSETenabled()) request.ICMND.msg3 |= 0x04;
   if (configuration.isRUBICSONenabled()  ) request.ICMND.msg3 |= 0x02;
   if (configuration.isAEenabled()        ) request.ICMND.msg3 |= 0x01;
   request.ICMND.msg4 = 0;
   if (configuration.isBLINDST1enabled()  ) request.ICMND.msg4 |= 0x80;
   if (configuration.isBLINDST0enabled()  ) request.ICMND.msg4 |= 0x40;
   if (configuration.isPROGUARDenabled()  ) request.ICMND.msg4 |= 0x20;
   if (configuration.isFS20enabled()      ) request.ICMND.msg4 |= 0x10;
   if (configuration.isLACROSSEenabled()  ) request.ICMND.msg4 |= 0x08;
   if (configuration.isHIDEKIenabled()    ) request.ICMND.msg4 |= 0x04;
   if (configuration.isLWRFenabled()      ) request.ICMND.msg4 |= 0x02;
   if (configuration.isMERTIKenabled()    ) request.ICMND.msg4 |= 0x01;
   request.ICMND.msg5 = 0;
   if (configuration.isVISONICenabled()   ) request.ICMND.msg5 |= 0x80;
   if (configuration.isATIenabled()       ) request.ICMND.msg5 |= 0x40;
   if (configuration.isOREGONenabled()    ) request.ICMND.msg5 |= 0x20;
   if (configuration.isMEIANTECHenabled() ) request.ICMND.msg5 |= 0x10;
   if (configuration.isHEEUenabled()      ) request.ICMND.msg5 |= 0x08;
   if (configuration.isACenabled()        ) request.ICMND.msg5 |= 0x04;
   if (configuration.isARCenabled()       ) request.ICMND.msg5 |= 0x02;
   if (configuration.isX10enabled()       ) request.ICMND.msg5 |= 0x01;

   return shared::communication::CByteBuffer((BYTE*)&request.ICMND, sizeof(request.ICMND));
}

const shared::communication::CByteBuffer CTransceiver::buildMessageToDevice(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceParametersTree) const
{
   try
   {
      unsigned char deviceType = deviceParametersTree.get<unsigned char>("type");

      // Create the RFXCom message
      switch(deviceType)
      {
      case pTypeLighting1:
         return rfxcomMessages::CLighting1(context, command, deviceParametersTree).encode(m_seqNumberProvider);
         break;
      case pTypeLighting2:
         return rfxcomMessages::CLighting2(context, command, deviceParametersTree).encode(m_seqNumberProvider);
         break;
      case pTypeLighting3:
         return rfxcomMessages::CLighting3(context, command, deviceParametersTree).encode(m_seqNumberProvider);
         break;
      case pTypeLighting4:
         return rfxcomMessages::CLighting4(context, command, deviceParametersTree).encode(m_seqNumberProvider);
         break;
      case pTypeLighting6:
         return rfxcomMessages::CLighting6(context, command, deviceParametersTree).encode(m_seqNumberProvider);
         break;
      case pTypeChime:
         return rfxcomMessages::CChime(context, command, deviceParametersTree).encode(m_seqNumberProvider);
         break;
      case pTypeCurtain:
         return rfxcomMessages::CCurtain1(context, command, deviceParametersTree).encode(m_seqNumberProvider);
         break;
      case pTypeFan:
         return rfxcomMessages::CFan(context, command, deviceParametersTree).encode(m_seqNumberProvider);
         break;
      case pTypeRFY:
         return rfxcomMessages::CRfy(context, command, deviceParametersTree).encode(m_seqNumberProvider);
         break;
      case pTypeFS20:
         return rfxcomMessages::CFS20(context, command, deviceParametersTree).encode(m_seqNumberProvider);
         break;
         //TODO compléter
      default:
         YADOMS_LOG(error) << "Invalid command \"" << command.serialize() << "\" : " << " unknown type " << deviceType;
         BOOST_ASSERT_MSG(false, "Invalid command (unknown type)");
         throw shared::exception::CInvalidParameter(command.serialize());
         break;
      }
   }
   catch (shared::exception::CException & e)
   {
      BOOST_ASSERT_MSG(false, "Invalid command (parameter doesn't exist)");
      std::string message = (boost::format("Invalid command \"%1%\" : %2%") % command.serialize() % e.what()).str();
      throw shared::exception::CInvalidParameter(message);
   }
}

boost::shared_ptr<rfxcomMessages::IRfxcomMessage> CTransceiver::decodeRfxcomMessage(boost::shared_ptr<yApi::IYadomsApi> context, const shared::communication::CByteBuffer& data) const
{
   const RBUF * const buf = reinterpret_cast<const RBUF* const>(data.content());

   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> message;
   switch(buf->RXRESPONSE.packettype)
   {
   case pTypeInterfaceMessage    : message.reset(new rfxcomMessages::CTransceiverStatus      (*buf, m_seqNumberProvider)); break;
   case pTypeRecXmitMessage      : message.reset(new rfxcomMessages::CAck                    (*buf, m_seqNumberProvider)); break;
   case pTypeRFXMeter            : message.reset(new rfxcomMessages::CRFXMeter               (context, *buf, m_seqNumberProvider)); break;
   case pTypeLighting1           : message.reset(new rfxcomMessages::CLighting1              (context, *buf, m_seqNumberProvider)); break;
   case pTypeLighting2           : message.reset(new rfxcomMessages::CLighting2              (context, *buf, m_seqNumberProvider)); break;
   case pTypeLighting3           : message.reset(new rfxcomMessages::CLighting3              (context, *buf, m_seqNumberProvider)); break;
   case pTypeLighting4           : message.reset(new rfxcomMessages::CLighting4              (context, *buf, m_seqNumberProvider)); break;
   case pTypeLighting6           : message.reset(new rfxcomMessages::CLighting6              (context, *buf, m_seqNumberProvider)); break;
   case pTypeChime               : message.reset(new rfxcomMessages::CChime                  (context, *buf, m_seqNumberProvider)); break;
   case pTypeCurtain             : message.reset(new rfxcomMessages::CCurtain1               (context, *buf, m_seqNumberProvider)); break;
   case pTypeFan                 : message.reset(new rfxcomMessages::CFan                    (context, *buf, m_seqNumberProvider)); break;
   case pTypeRFY                 : message.reset(new rfxcomMessages::CRfy                    (context, *buf, m_seqNumberProvider)); break;
   case pTypeTEMP                : message.reset(new rfxcomMessages::CTemp                   (context, *buf, m_seqNumberProvider)); break;
   case pTypeHUM                 : message.reset(new rfxcomMessages::CHumidity               (context, *buf, m_seqNumberProvider)); break;
   case pTypeTEMP_HUM            : message.reset(new rfxcomMessages::CTempHumidity           (context, *buf, m_seqNumberProvider)); break;
   case pTypeBARO                : message.reset(new rfxcomMessages::CBarometric             (context, *buf, m_seqNumberProvider)); break;
   case pTypeTEMP_HUM_BARO       : message.reset(new rfxcomMessages::CTempHumidityBarometric (context, *buf, m_seqNumberProvider)); break;
   case pTypeRAIN                : message.reset(new rfxcomMessages::CRain                   (context, *buf, m_seqNumberProvider)); break;
   case pTypeWIND                : message.reset(new rfxcomMessages::CWind                   (context, *buf, m_seqNumberProvider)); break;
   case pTypeUV                  : message.reset(new rfxcomMessages::CUV                     (context, *buf, m_seqNumberProvider)); break;
   case pTypeCURRENT             : message.reset(new rfxcomMessages::CCurrent                (context, *buf, m_seqNumberProvider)); break;
   case pTypeENERGY              : message.reset(new rfxcomMessages::CEnergy                 (context, *buf, m_seqNumberProvider)); break;
   case pTypeCURRENTENERGY       : message.reset(new rfxcomMessages::CCurrentEnergy          (context, *buf, m_seqNumberProvider)); break;
   case pTypePOWER               : message.reset(new rfxcomMessages::CPower                  (context, *buf, m_seqNumberProvider)); break;
   case pTypeWEIGHT              : message.reset(new rfxcomMessages::CWeight                 (context, *buf, m_seqNumberProvider)); break;
   case pTypeRFXSensor           : message.reset(new rfxcomMessages::CRFXSensor              (context, *buf, m_seqNumberProvider)); break;
   case pTypeFS20                : message.reset(new rfxcomMessages::CFS20                   (context, *buf, m_seqNumberProvider)); break;
      // TODO à compléter
   default:
      {
         YADOMS_LOG(error) << "Invalid RfxCom message received, unknown packet type " << std::setfill('0') << std::setw(sizeof(unsigned char) * 2) << std::hex << (int)buf->RXRESPONSE.packettype;
         break;
      }
   }

   return message;
}

bool CTransceiver::createDeviceManually(boost::shared_ptr<yApi::IYadomsApi> context, const boost::shared_ptr<yApi::IManuallyDeviceCreationData> data) const
{
   try
   {
      if (data->getConfiguration().get<bool>("type.content.x10.radio"))
         rfxcomMessages::CLighting1 msg(context, sTypeX10, data->getConfiguration().get<shared::CDataContainer>("type.content.x10.content"));
      else if (data->getConfiguration().get<bool>("type.content.arc.radio"))
         rfxcomMessages::CLighting1 msg(context, sTypeARC, data->getConfiguration().get<shared::CDataContainer>("type.content.arc.content"));
      else if (data->getConfiguration().get<bool>("type.content.ab400d.radio"))
         rfxcomMessages::CLighting1 msg(context, sTypeAB400D, data->getConfiguration().get<shared::CDataContainer>("type.content.ab400d.content"));
      else if (data->getConfiguration().get<bool>("type.content.waveman.radio"))
         rfxcomMessages::CLighting1 msg(context, sTypeWaveman, data->getConfiguration().get<shared::CDataContainer>("type.content.waveman.content"));
      else if (data->getConfiguration().get<bool>("type.content.emw200.radio"))
         rfxcomMessages::CLighting1 msg(context, sTypeEMW200, data->getConfiguration().get<shared::CDataContainer>("type.content.emw200.content"));
      else if (data->getConfiguration().get<bool>("type.content.impuls.radio"))
         rfxcomMessages::CLighting1 msg(context, sTypeIMPULS, data->getConfiguration().get<shared::CDataContainer>("type.content.impuls.content"));
      else if (data->getConfiguration().get<bool>("type.content.risingSun.radio"))
         rfxcomMessages::CLighting1 msg(context, sTypeRisingSun, data->getConfiguration().get<shared::CDataContainer>("type.content.risingSun.content"));
      else if (data->getConfiguration().get<bool>("type.content.philips.radio"))
         rfxcomMessages::CLighting1 msg(context, sTypePhilips, data->getConfiguration().get<shared::CDataContainer>("type.content.philips.content"));
      else if (data->getConfiguration().get<bool>("type.content.energenie.radio"))
         rfxcomMessages::CLighting1 msg(context, sTypeEnergenie, data->getConfiguration().get<shared::CDataContainer>("type.content.energenie.content"));
      else if (data->getConfiguration().get<bool>("type.content.energenie5.radio"))
         rfxcomMessages::CLighting1 msg(context, sTypeEnergenie5, data->getConfiguration().get<shared::CDataContainer>("type.content.energenie5.content"));
      else if (data->getConfiguration().get<bool>("type.content.gdr2.radio"))
         rfxcomMessages::CLighting1 msg(context, sTypeGDR2, data->getConfiguration().get<shared::CDataContainer>("type.content.gdr2.content"));

      else if (data->getConfiguration().get<bool>("type.content.blyss.radio"))
         rfxcomMessages::CLighting6 msg(context, sTypeBlyss, data->getConfiguration().get<shared::CDataContainer>("type.content.blyss.content"));

      // TODO à compléter (voir Domoticz "Switch_Type_Desc")

      else
         throw shared::exception::CInvalidParameter("Unknown device type");
   }
   catch(shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Fail to create device manually, invalid parameter " << e.what();
      return false;
   }
   catch(shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Fail to create device manually, out of range " << e.what();
      return false;
   }

   return true;
}
