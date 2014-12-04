#include "stdafx.h"
#include "Transceiver.h"
#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/exception/InvalidParameter.hpp>
#include "RfxcomFactory.h"
#include "rfxcomMessages/Ack.h"
#include "rfxcomMessages/Barometric.h"
#include "rfxcomMessages/Blinds1.h"
#include "rfxcomMessages/Bbq.h"
#include "rfxcomMessages/Camera1.h"
#include "rfxcomMessages/Chime.h"
#include "rfxcomMessages/Current.h"
#include "rfxcomMessages/CurrentEnergy.h"
#include "rfxcomMessages/Curtain1.h"
#include "rfxcomMessages/DateTime.h"
#include "rfxcomMessages/Fan.h"
#include "rfxcomMessages/Energy.h"
#include "rfxcomMessages/FS20.h"
#include "rfxcomMessages/Humidity.h"
#include "rfxcomMessages/Lighting1.h"
#include "rfxcomMessages/Lighting2.h"
#include "rfxcomMessages/Lighting3.h"
#include "rfxcomMessages/Lighting4.h"
#include "rfxcomMessages/Lighting5.h"
#include "rfxcomMessages/Lighting6.h"
#include "rfxcomMessages/Power.h"
#include "rfxcomMessages/Rain.h"
#include "rfxcomMessages/Remote.h"
#include "rfxcomMessages/RFXMeter.h"
#include "rfxcomMessages/RFXSensor.h"
#include "rfxcomMessages/Rfy.h"
#include "rfxcomMessages/Security1.h"
#include "rfxcomMessages/Temp.h"
#include "rfxcomMessages/TempRain.h"
#include "rfxcomMessages/TempHumidity.h"
#include "rfxcomMessages/TempHumidityBarometric.h"
#include "rfxcomMessages/Thermostat1.h"
#include "rfxcomMessages/Thermostat2.h"
#include "rfxcomMessages/Thermostat3.h"
#include "rfxcomMessages/TransceiverStatus.h"
#include "rfxcomMessages/UV.h"
#include "rfxcomMessages/Weight.h"
#include "rfxcomMessages/Wind.h"
#include "IncrementSequenceNumber.h"
#include <shared/communication/PortException.hpp>
#include "ProtocolException.hpp"
#include "ManuallyDeviceCreationException.hpp"

//
// =======================================================================
// RFXCOM implementation
// =======================================================================
// This RFXCom support was developped for :
// - Sepcifications "RFXtrx SDK.pdf" : Version 6.26 October 9, 2014
// - RFXtrx.h : version 6.26
// =======================================================================
//

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

   return toBuffer(request, GET_RBUF_STRUCT_SIZE(ICMND));
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

   return toBuffer(request, GET_RBUF_STRUCT_SIZE(ICMND));
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

   return toBuffer(request, GET_RBUF_STRUCT_SIZE(ICMND));
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CTransceiver::buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<const yApi::IDeviceCommand> command) const
{
   try
   {
      const shared::CDataContainer& deviceDetails = context->getDeviceDetails(command->getTargetDevice());
      unsigned char deviceType = deviceDetails.get<unsigned char>("type");

      // Create the RFXCom message
      switch(deviceType)
      {
      case pTypeLighting1:
         return rfxcomMessages::CLighting1(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeLighting2:
         return rfxcomMessages::CLighting2(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeLighting3:
         return rfxcomMessages::CLighting3(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeLighting4:
         return rfxcomMessages::CLighting4(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeLighting5:
         return rfxcomMessages::CLighting5(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeLighting6:
         return rfxcomMessages::CLighting6(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeChime:
         return rfxcomMessages::CChime(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeCurtain:
         return rfxcomMessages::CCurtain1(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeFan:
         return rfxcomMessages::CFan(context, command->getKeyword(), command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeBlinds:
         return rfxcomMessages::CBlinds1(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeRFY:
         return rfxcomMessages::CRfy(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeSecurity1:
         return rfxcomMessages::CSecurity1(context, command->getKeyword(), command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeCamera:
         return rfxcomMessages::CCamera1(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeRemote:
         return rfxcomMessages::CRemote(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeThermostat1:
         return rfxcomMessages::CThermostat1(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeThermostat2:
         return rfxcomMessages::CThermostat2(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeThermostat3:
         return rfxcomMessages::CThermostat3(context, command->getKeyword(), command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      case pTypeFS20:
         return rfxcomMessages::CFS20(context, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
         break;
      default:
         YADOMS_LOG(error) << "Invalid command \"" << command->getBody() << "\" : " << " unknown type " << deviceType;
         BOOST_ASSERT_MSG(false, "Invalid command (unknown type)");
         throw shared::exception::CInvalidParameter(command->getBody());
         break;
      }
   }
   catch (shared::exception::CException & e)
   {
      BOOST_ASSERT_MSG(false, "Invalid command (parameter doesn't exist)");
      std::string message = (boost::format("Invalid command \"%1%\" : %2%") % command->getBody() % e.what()).str();
      throw shared::exception::CInvalidParameter(message);
   }
}

boost::shared_ptr<rfxcomMessages::IRfxcomMessage> CTransceiver::decodeRfxcomMessage(boost::shared_ptr<yApi::IYPluginApi> context, const shared::communication::CByteBuffer& data) const
{
   const RBUF * const buf = reinterpret_cast<const RBUF* const>(data.begin());

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
   case pTypeLighting5           : message.reset(new rfxcomMessages::CLighting5              (context, *buf, m_seqNumberProvider)); break;
   case pTypeLighting6           : message.reset(new rfxcomMessages::CLighting6              (context, *buf, m_seqNumberProvider)); break;
   case pTypeChime               : message.reset(new rfxcomMessages::CChime                  (context, *buf, m_seqNumberProvider)); break;
   case pTypeFan                 : message.reset(new rfxcomMessages::CFan                    (context, *buf, m_seqNumberProvider)); break;
   case pTypeCurtain             : message.reset(new rfxcomMessages::CCurtain1               (context, *buf, m_seqNumberProvider)); break;
   case pTypeBlinds              : message.reset(new rfxcomMessages::CBlinds1                (context, *buf, m_seqNumberProvider)); break;
   case pTypeRFY                 : message.reset(new rfxcomMessages::CRfy                    (context, *buf, m_seqNumberProvider)); break;
   case pTypeTEMP_RAIN           : message.reset(new rfxcomMessages::CTempRain               (context, *buf, m_seqNumberProvider)); break;
   case pTypeTEMP                : message.reset(new rfxcomMessages::CTemp                   (context, *buf, m_seqNumberProvider)); break;
   case pTypeHUM                 : message.reset(new rfxcomMessages::CHumidity               (context, *buf, m_seqNumberProvider)); break;
   case pTypeTEMP_HUM            : message.reset(new rfxcomMessages::CTempHumidity           (context, *buf, m_seqNumberProvider)); break;
   case pTypeBARO                : message.reset(new rfxcomMessages::CBarometric             (context, *buf, m_seqNumberProvider)); break;
   case pTypeTEMP_HUM_BARO       : message.reset(new rfxcomMessages::CTempHumidityBarometric (context, *buf, m_seqNumberProvider)); break;
   case pTypeRAIN                : message.reset(new rfxcomMessages::CRain                   (context, *buf, m_seqNumberProvider)); break;
   case pTypeWIND                : message.reset(new rfxcomMessages::CWind                   (context, *buf, m_seqNumberProvider)); break;
   case pTypeUV                  : message.reset(new rfxcomMessages::CUV                     (context, *buf, m_seqNumberProvider)); break;
   case pTypeDT                  : message.reset(new rfxcomMessages::CDateTime               (context, *buf, m_seqNumberProvider)); break;
   case pTypeCURRENT             : message.reset(new rfxcomMessages::CCurrent                (context, *buf, m_seqNumberProvider)); break;
   case pTypeENERGY              : message.reset(new rfxcomMessages::CEnergy                 (context, *buf, m_seqNumberProvider)); break;
   case pTypeCURRENTENERGY       : message.reset(new rfxcomMessages::CCurrentEnergy          (context, *buf, m_seqNumberProvider)); break;
   case pTypePOWER               : message.reset(new rfxcomMessages::CPower                  (context, *buf, m_seqNumberProvider)); break;
   case pTypeWEIGHT              : message.reset(new rfxcomMessages::CWeight                 (context, *buf, m_seqNumberProvider)); break;
   case pTypeRFXSensor           : message.reset(new rfxcomMessages::CRFXSensor              (context, *buf, m_seqNumberProvider)); break;
   case pTypeSecurity1           : message.reset(new rfxcomMessages::CSecurity1              (context, *buf, m_seqNumberProvider)); break;
   case pTypeCamera              : message.reset(new rfxcomMessages::CCamera1                (context, *buf, m_seqNumberProvider)); break;
   case pTypeRemote              : message.reset(new rfxcomMessages::CRemote                 (context, *buf, m_seqNumberProvider)); break;
   case pTypeThermostat1         : message.reset(new rfxcomMessages::CThermostat1            (context, *buf, m_seqNumberProvider)); break;
   case pTypeThermostat2         : message.reset(new rfxcomMessages::CThermostat2            (context, *buf, m_seqNumberProvider)); break;
   case pTypeThermostat3         : message.reset(new rfxcomMessages::CThermostat3            (context, *buf, m_seqNumberProvider)); break;
   case pTypeBBQ                 : message.reset(new rfxcomMessages::CBbq                    (context, *buf, m_seqNumberProvider)); break;
   case pTypeFS20                : message.reset(new rfxcomMessages::CFS20                   (context, *buf, m_seqNumberProvider)); break;
   default:
      {
         YADOMS_LOG(error) << "Invalid RfxCom message received, unknown packet type " << std::setfill('0') << std::setw(sizeof(unsigned char) * 2) << std::hex << (int)buf->RXRESPONSE.packettype;
         break;
      }
   }

   return message;
}

const std::string CTransceiver::createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> context, const yApi::IManuallyDeviceCreationData& data) const
{
   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> msg;
   try
   {
      // Lighting1
      if      (data.getConfiguration().get<bool>("type.content.x10.radio"))
         msg.reset(new rfxcomMessages::CLighting1(context, sTypeX10, data.getConfiguration().get<shared::CDataContainer>("type.content.x10.content")));
      else if (data.getConfiguration().get<bool>("type.content.arc.radio"))
         msg.reset(new rfxcomMessages::CLighting1(context, sTypeARC, data.getConfiguration().get<shared::CDataContainer>("type.content.arc.content")));
      else if (data.getConfiguration().get<bool>("type.content.ab400d.radio"))
         msg.reset(new rfxcomMessages::CLighting1(context, sTypeAB400D, data.getConfiguration().get<shared::CDataContainer>("type.content.ab400d.content")));
      else if (data.getConfiguration().get<bool>("type.content.waveman.radio"))
         msg.reset(new rfxcomMessages::CLighting1(context, sTypeWaveman, data.getConfiguration().get<shared::CDataContainer>("type.content.waveman.content")));
      else if (data.getConfiguration().get<bool>("type.content.emw200.radio"))
         msg.reset(new rfxcomMessages::CLighting1(context, sTypeEMW200, data.getConfiguration().get<shared::CDataContainer>("type.content.emw200.content")));
      else if (data.getConfiguration().get<bool>("type.content.impuls.radio"))
         msg.reset(new rfxcomMessages::CLighting1(context, sTypeIMPULS, data.getConfiguration().get<shared::CDataContainer>("type.content.impuls.content")));
      else if (data.getConfiguration().get<bool>("type.content.risingSun.radio"))
         msg.reset(new rfxcomMessages::CLighting1(context, sTypeRisingSun, data.getConfiguration().get<shared::CDataContainer>("type.content.risingSun.content")));
      else if (data.getConfiguration().get<bool>("type.content.philips.radio"))
         msg.reset(new rfxcomMessages::CLighting1(context, sTypePhilips, data.getConfiguration().get<shared::CDataContainer>("type.content.philips.content")));
      else if (data.getConfiguration().get<bool>("type.content.energenie.radio"))
         msg.reset(new rfxcomMessages::CLighting1(context, sTypeEnergenie, data.getConfiguration().get<shared::CDataContainer>("type.content.energenie.content")));
      else if (data.getConfiguration().get<bool>("type.content.energenie5.radio"))
         msg.reset(new rfxcomMessages::CLighting1(context, sTypeEnergenie5, data.getConfiguration().get<shared::CDataContainer>("type.content.energenie5.content")));
      else if (data.getConfiguration().get<bool>("type.content.gdr2.radio"))
         msg.reset(new rfxcomMessages::CLighting1(context, sTypeGDR2, data.getConfiguration().get<shared::CDataContainer>("type.content.gdr2.content")));

      // Lighting2
      else if (data.getConfiguration().get<bool>("type.content.ac.radio"))
         msg.reset(new rfxcomMessages::CLighting2(context, sTypeX10, data.getConfiguration().get<shared::CDataContainer>("type.content.ac.content")));
      else if (data.getConfiguration().get<bool>("type.content.homeEasyEU.radio"))
         msg.reset(new rfxcomMessages::CLighting2(context, sTypeARC, data.getConfiguration().get<shared::CDataContainer>("type.content.homeEasyEU.content")));
      else if (data.getConfiguration().get<bool>("type.content.anslut.radio"))
         msg.reset(new rfxcomMessages::CLighting2(context, sTypeARC, data.getConfiguration().get<shared::CDataContainer>("type.content.anslut.content")));
      else if (data.getConfiguration().get<bool>("type.content.kambrookRf3672.radio"))
         msg.reset(new rfxcomMessages::CLighting2(context, sTypeARC, data.getConfiguration().get<shared::CDataContainer>("type.content.KambrookRf3672.content")));

      // Lighting3
      else if (data.getConfiguration().get<bool>("type.content.koppla.radio"))
         msg.reset(new rfxcomMessages::CLighting3(context, sTypeKoppla, data.getConfiguration().get<shared::CDataContainer>("type.content.koppla.content")));

      // Lighting4
      else if (data.getConfiguration().get<bool>("type.content.pt2262.radio"))
         msg.reset(new rfxcomMessages::CLighting4(context, sTypePT2262, data.getConfiguration().get<shared::CDataContainer>("type.content.pt2262.content")));

      // Lighting5
      else if (data.getConfiguration().get<bool>("type.content.lightwaveRf.radio"))
         msg.reset(new rfxcomMessages::CLighting5(context, sTypeLightwaveRF, data.getConfiguration().get<shared::CDataContainer>("type.content.lightwaveRf.content")));
      else if (data.getConfiguration().get<bool>("type.content.emw100.radio"))
         msg.reset(new rfxcomMessages::CLighting5(context, sTypeEMW100, data.getConfiguration().get<shared::CDataContainer>("type.content.emw100.content")));
      else if (data.getConfiguration().get<bool>("type.content.bbsb.radio"))
         msg.reset(new rfxcomMessages::CLighting5(context, sTypeBBSB, data.getConfiguration().get<shared::CDataContainer>("type.content.bbsb.content")));
      else if (data.getConfiguration().get<bool>("type.content.mdRemote.radio"))
         msg.reset(new rfxcomMessages::CLighting5(context, sTypeMDREMOTE, data.getConfiguration().get<shared::CDataContainer>("type.content.mdRemote.content")));
      else if (data.getConfiguration().get<bool>("type.content.rsl.radio"))
         msg.reset(new rfxcomMessages::CLighting5(context, sTypeRSL, data.getConfiguration().get<shared::CDataContainer>("type.content.rsl.content")));
      else if (data.getConfiguration().get<bool>("type.content.livolo.radio"))
         msg.reset(new rfxcomMessages::CLighting5(context, sTypeLivolo, data.getConfiguration().get<shared::CDataContainer>("type.content.livolo.content")));
      else if (data.getConfiguration().get<bool>("type.content.trc02.radio"))
         msg.reset(new rfxcomMessages::CLighting5(context, sTypeTRC02, data.getConfiguration().get<shared::CDataContainer>("type.content.trc02.content")));
      else if (data.getConfiguration().get<bool>("type.content.aoke.radio"))
         msg.reset(new rfxcomMessages::CLighting5(context, sTypeAoke, data.getConfiguration().get<shared::CDataContainer>("type.content.aoke.content")));
      else if (data.getConfiguration().get<bool>("type.content.trc02_2.radio"))
         msg.reset(new rfxcomMessages::CLighting5(context, sTypeTRC02_2, data.getConfiguration().get<shared::CDataContainer>("type.content.trc02_2.content")));
      else if (data.getConfiguration().get<bool>("type.content.eurodomest.radio"))
         msg.reset(new rfxcomMessages::CLighting5(context, sTypeEurodomest, data.getConfiguration().get<shared::CDataContainer>("type.content.eurodomest.content")));

      // Lighting6
      else if (data.getConfiguration().get<bool>("type.content.blyss.radio"))
         msg.reset(new rfxcomMessages::CLighting6(context, sTypeBlyss, data.getConfiguration().get<shared::CDataContainer>("type.content.blyss.content")));

      // Chime
      else if (data.getConfiguration().get<bool>("type.content.byronSx.radio"))
         msg.reset(new rfxcomMessages::CChime(context, sTypeByronSX, data.getConfiguration().get<shared::CDataContainer>("type.content.byronSx.content")));
      else if (data.getConfiguration().get<bool>("type.content.byronMp001.radio"))
         msg.reset(new rfxcomMessages::CChime(context, sTypeByronMP001, data.getConfiguration().get<shared::CDataContainer>("type.content.byronMp001.content")));

      // Fan
      else if (data.getConfiguration().get<bool>("type.content.siemensSf01.radio"))
         msg.reset(new rfxcomMessages::CFan(context, sTypeSiemensSF01, data.getConfiguration().get<shared::CDataContainer>("type.content.siemensSf01.content")));

      // Curtain1
      else if (data.getConfiguration().get<bool>("type.content.harrisonCurtain.radio"))
         msg.reset(new rfxcomMessages::CCurtain1(context, sTypeHarrison, data.getConfiguration().get<shared::CDataContainer>("type.content.harrisonCurtain.content")));

      // Blinds1
      else if (data.getConfiguration().get<bool>("type.content.rollerTrolHastaNew.radio"))
         msg.reset(new rfxcomMessages::CBlinds1(context, sTypeBlindsT0, data.getConfiguration().get<shared::CDataContainer>("type.content.rollerTrolHastaNew.content")));
      else if (data.getConfiguration().get<bool>("type.content.hastaOld.radio"))
         msg.reset(new rfxcomMessages::CBlinds1(context, sTypeBlindsT1, data.getConfiguration().get<shared::CDataContainer>("type.content.hastaOld.content")));
      else if (data.getConfiguration().get<bool>("type.content.aOkRf01.radio"))
         msg.reset(new rfxcomMessages::CBlinds1(context, sTypeBlindsT2, data.getConfiguration().get<shared::CDataContainer>("type.content.aOkRf01.content")));
      else if (data.getConfiguration().get<bool>("type.content.aOkAc114.radio"))
         msg.reset(new rfxcomMessages::CBlinds1(context, sTypeBlindsT3, data.getConfiguration().get<shared::CDataContainer>("type.content.aOkAc114.content")));
      else if (data.getConfiguration().get<bool>("type.content.raex.radio"))
         msg.reset(new rfxcomMessages::CBlinds1(context, sTypeBlindsT4, data.getConfiguration().get<shared::CDataContainer>("type.content.raex.content")));
      else if (data.getConfiguration().get<bool>("type.content.mediaMount.radio"))
         msg.reset(new rfxcomMessages::CBlinds1(context, sTypeBlindsT5, data.getConfiguration().get<shared::CDataContainer>("type.content.mediaMount.content")));
      else if (data.getConfiguration().get<bool>("type.content.dc106.radio"))
         msg.reset(new rfxcomMessages::CBlinds1(context, sTypeBlindsT6, data.getConfiguration().get<shared::CDataContainer>("type.content.dc106.content")));
      else if (data.getConfiguration().get<bool>("type.content.forest.radio"))
         msg.reset(new rfxcomMessages::CBlinds1(context, sTypeBlindsT7, data.getConfiguration().get<shared::CDataContainer>("type.content.forest.content")));

      // Rfy
      else if (data.getConfiguration().get<bool>("type.content.rfy.radio"))
         msg.reset(new rfxcomMessages::CRfy(context, sTypeRFY, data.getConfiguration().get<shared::CDataContainer>("type.content.rfy.content")));
      else if (data.getConfiguration().get<bool>("type.content.rfyExt.radio"))
         msg.reset(new rfxcomMessages::CRfy(context, sTypeRFYext, data.getConfiguration().get<shared::CDataContainer>("type.content.rfyExt.content")));

      // Security1
      else if (data.getConfiguration().get<bool>("type.content.x10SecurityR.radio"))
         msg.reset(new rfxcomMessages::CSecurity1(context, sTypeSecX10R, data.getConfiguration().get<shared::CDataContainer>("type.content.x10SecurityR.content")));

      // Camera1
      else if (data.getConfiguration().get<bool>("type.content.cameraX10Ninja.radio"))
         msg.reset(new rfxcomMessages::CCamera1(context, sTypeNinja, data.getConfiguration().get<shared::CDataContainer>("type.content.cameraX10Ninja.content")));

      // Thermostat1
      else if (data.getConfiguration().get<bool>("type.content.digimax.radio"))
         msg.reset(new rfxcomMessages::CThermostat1(context, sTypeDigimax, data.getConfiguration().get<shared::CDataContainer>("type.content.digimax.content")));
      else if (data.getConfiguration().get<bool>("type.content.digimaxShort.radio"))
         msg.reset(new rfxcomMessages::CThermostat1(context, sTypeDigimaxShort, data.getConfiguration().get<shared::CDataContainer>("type.content.digimaxShort.content")));

      // Thermostat2
      else if (data.getConfiguration().get<bool>("type.content.he105.radio"))
         msg.reset(new rfxcomMessages::CThermostat2(context, sTypeHE105, data.getConfiguration().get<shared::CDataContainer>("type.content.he105.content")));
      else if (data.getConfiguration().get<bool>("type.content.rts10.radio"))
         msg.reset(new rfxcomMessages::CThermostat2(context, sTypeRTS10, data.getConfiguration().get<shared::CDataContainer>("type.content.rts10.content")));

      // Thermostat3
      else if (data.getConfiguration().get<bool>("type.content.g6rH4t1.radio"))
         msg.reset(new rfxcomMessages::CThermostat3(context, sTypeMertikG6RH4T1, data.getConfiguration().get<shared::CDataContainer>("type.content.g6rH4t1.content")));
      else if (data.getConfiguration().get<bool>("type.content.g6rH4tb.radio"))
         msg.reset(new rfxcomMessages::CThermostat3(context, sTypeMertikG6RH4TB, data.getConfiguration().get<shared::CDataContainer>("type.content.g6rH4tb.content")));

      // FS20
      else if (data.getConfiguration().get<bool>("type.content.fs20.radio"))
         msg.reset(new rfxcomMessages::CFS20(context, sTypeFS20, data.getConfiguration().get<shared::CDataContainer>("type.content.fs20.content")));
      else if (data.getConfiguration().get<bool>("type.content.fht8v.radio"))
         msg.reset(new rfxcomMessages::CFS20(context, sTypeFHT8V, data.getConfiguration().get<shared::CDataContainer>("type.content.fht8v.content")));
      else if (data.getConfiguration().get<bool>("type.content.fht80.radio"))
         msg.reset(new rfxcomMessages::CFS20(context, sTypeFHT80, data.getConfiguration().get<shared::CDataContainer>("type.content.fht80.content")));

      else
         throw shared::exception::CInvalidParameter("Unknown device type");
   }
   catch(shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Fail to create device manually, invalid parameter : " << e.what();
      YADOMS_LOG(error) << "data : " << data.getConfiguration().get<shared::CDataContainer>("type.content").serialize();
      throw CManuallyDeviceCreationException("invalid parameter");
   }
   catch(shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Fail to create device manually, out of range : " << e.what();
      YADOMS_LOG(error) << "data : " << data.getConfiguration().get<shared::CDataContainer>("type.content").serialize();
      throw CManuallyDeviceCreationException("out of range");
   }

   return msg->getDeviceName();
}
