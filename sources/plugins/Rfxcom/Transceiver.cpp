#include "stdafx.h"
#include "Transceiver.h"
#include <shared/DataContainer.h>
#include <shared/exception/InvalidParameter.hpp>
#include "rfxcomMessages/Ack.h"
#include "rfxcomMessages/Barometric.h"
#include "rfxcomMessages/Blinds1.h"
#include "rfxcomMessages/Bbq.h"
#include "rfxcomMessages/Camera1.h"
#include "rfxcomMessages/Cartelectronic.h"
#include "rfxcomMessages/Chime.h"
#include "rfxcomMessages/Current.h"
#include "rfxcomMessages/CurrentEnergy.h"
#include "rfxcomMessages/Curtain1.h"
#include "rfxcomMessages/DateTime.h"
#include "rfxcomMessages/Fan.h"
#include "rfxcomMessages/Energy.h"
#include "rfxcomMessages/FS20.h"
#include "rfxcomMessages/HomeConfort.h"
#include "rfxcomMessages/Humidity.h"
#include "rfxcomMessages/Lighting1.h"
#include "rfxcomMessages/Lighting2.h"
#include "rfxcomMessages/Lighting3.h"
#include "rfxcomMessages/Lighting4.h"
#include "rfxcomMessages/Lighting5.h"
#include "rfxcomMessages/Lighting6.h"
#include "rfxcomMessages/Power.h"
#include "rfxcomMessages/Radiator1.h"
#include "rfxcomMessages/Rain.h"
#include "rfxcomMessages/Remote.h"
#include "rfxcomMessages/RFXMeter.h"
#include "rfxcomMessages/RFXSensor.h"
#include "rfxcomMessages/Rfy.h"
#include "rfxcomMessages/Security1.h"
#include "rfxcomMessages/Security2.h"
#include "rfxcomMessages/Temp.h"
#include "rfxcomMessages/TempRain.h"
#include "rfxcomMessages/TempHumidity.h"
#include "rfxcomMessages/TempHumidityBarometric.h"
#include "rfxcomMessages/Thermostat1.h"
#include "rfxcomMessages/Thermostat2.h"
#include "rfxcomMessages/Thermostat3.h"
#include "rfxcomMessages/Thermostat4.h"
#include "rfxcomMessages/TransceiverStatus.h"
#include "rfxcomMessages/UV.h"
#include "rfxcomMessages/Weight.h"
#include "rfxcomMessages/Wind.h"
#include "IncrementSequenceNumber.h"
#include "ManuallyDeviceCreationException.hpp"
#include "MessageFilteredException.hpp"
#include <shared/Log.h>

//
// =======================================================================
// RFXCOM implementation
// =======================================================================
// This RFXCom support was developped for :
// - Sepcifications "RFXtrx SDK.pdf" : Version 9.17 Oct ??, 2017
// - RFXtrx.h : version 9.17
// =======================================================================
//

const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> rfxcomMessages::IRfxcomMessage::NoKeywords;

CTransceiver::CTransceiver(boost::shared_ptr<IPairingHelper> pairingHelper)
   : m_pairingHelper(pairingHelper),
     m_seqNumberProvider(boost::make_shared<CIncrementSequenceNumber>()),
     m_unsecuredProtocolFilters(createUnsecuredProtocolFilters())
{
}

CTransceiver::~CTransceiver()
{
}

std::map<int, boost::shared_ptr<IUnsecuredProtocolFilter>> CTransceiver::createUnsecuredProtocolFilters()
{
   std::map<int, boost::shared_ptr<IUnsecuredProtocolFilter>> filters;
   filters[pTypeCURRENTENERGY] = rfxcomMessages::CCurrentEnergy::createFilter();
   filters[pTypeLighting4] = rfxcomMessages::CLighting4::createFilter();
   filters[pTypeSecurity1] = rfxcomMessages::CSecurity1::createFilter();
   filters[pTypeLighting5] = rfxcomMessages::CSecurity1::createFilter();
   return filters;
}


shared::communication::CByteBuffer CTransceiver::buildResetCmd() const
{
   // Raz sequence number
   m_seqNumberProvider->reset();

   RBUF request;
   MEMCLEAR(request.ICMND); // For better performance, just clear the needed sub-structure of RBUF

   request.ICMND.packetlength = ENCODE_PACKET_LENGTH(ICMND);
   request.ICMND.packettype = pTypeInterfaceControl;
   request.ICMND.subtype = sTypeInterfaceCommand;
   request.ICMND.seqnbr = m_seqNumberProvider->last();
   request.ICMND.cmnd = cmdRESET;

   return toBuffer(request, GET_RBUF_STRUCT_SIZE(ICMND));
}

shared::communication::CByteBuffer CTransceiver::buildGetStatusCmd() const
{
   RBUF request;
   MEMCLEAR(request.ICMND); // For better performance, just clear the needed sub-structure of RBUF

   request.ICMND.packetlength = ENCODE_PACKET_LENGTH(ICMND);
   request.ICMND.packettype = pTypeInterfaceControl;
   request.ICMND.subtype = sTypeInterfaceCommand;
   request.ICMND.seqnbr = m_seqNumberProvider->next();
   request.ICMND.cmnd = cmdSTATUS;

   return toBuffer(request, GET_RBUF_STRUCT_SIZE(ICMND));
}

shared::communication::CByteBuffer CTransceiver::buildSetModeCmd(unsigned char frequency,
                                                                 const IRfxcomConfiguration& configuration) const
{
   RBUF request;
   MEMCLEAR(request.ICMND); // For better performance, just clear the needed sub-structure of RBUF

   request.ICMND.packetlength = ENCODE_PACKET_LENGTH(ICMND);
   request.ICMND.packettype = pTypeInterfaceControl;
   request.ICMND.subtype = sTypeInterfaceCommand;
   request.ICMND.seqnbr = m_seqNumberProvider->next();
   request.ICMND.cmnd = cmdSETMODE;

   // Frequency
   request.ICMND.freqsel = frequency;

   // Frequency
   request.ICMND.xmitpwr = 0; // Not yet supported by RFXCom

   // Add protocols activation
   request.ICMND.msg3 = 0;
   if (configuration.isAEenabled()) request.ICMND.msg3 |= msg3_AE;
   if (configuration.isRUBICSONenabled()) request.ICMND.msg3 |= msg3_RUBICSON;
   if (configuration.isFINEOFFSETenabled()) request.ICMND.msg3 |= msg3_FINEOFFSET;
   if (configuration.isLIGHTING4enabled()) request.ICMND.msg3 |= msg3_LIGHTING4;
   if (configuration.isRSLenabled()) request.ICMND.msg3 |= msg3_RSL;
   if (configuration.isSXenabled()) request.ICMND.msg3 |= msg3_SX;
   if (configuration.isIMAGINTRONIXenabled()) request.ICMND.msg3 |= msg3_IMAGINTRONIX;
   if (configuration.isUNDECODEDenabled()) request.ICMND.msg3 |= msg3_undec;
   request.ICMND.msg4 = 0;
   if (configuration.isMERTIKenabled()) request.ICMND.msg4 |= msg4_MERTIK;
   if (configuration.isLWRFenabled()) request.ICMND.msg4 |= msg4_AD;
   if (configuration.isHIDEKIenabled()) request.ICMND.msg4 |= msg4_HID;
   if (configuration.isLACROSSEenabled()) request.ICMND.msg4 |= msg4_LCROS;
   if (configuration.isLEGRANDenabled()) request.ICMND.msg4 |= msg4_LEGRAND;
   if (configuration.isBLINDST0enabled()) request.ICMND.msg4 |= msg4_BLINDST0;
   if (configuration.isBLINDST1enabled()) request.ICMND.msg4 |= msg4_BLINDST1;
   request.ICMND.msg5 = 0;
   if (configuration.isX10enabled()) request.ICMND.msg5 |= msg5_X10;
   if (configuration.isARCenabled()) request.ICMND.msg5 |= msg5_ARC;
   if (configuration.isACenabled()) request.ICMND.msg5 |= msg5_AC;
   if (configuration.isHEEUenabled()) request.ICMND.msg5 |= msg5_HEU;
   if (configuration.isMEIANTECHenabled()) request.ICMND.msg5 |= msg5_MEI;
   if (configuration.isOREGONenabled()) request.ICMND.msg5 |= msg5_OREGON;
   if (configuration.isATIenabled()) request.ICMND.msg5 |= msg5_ATI;
   if (configuration.isVISONICenabled()) request.ICMND.msg5 |= msg5_VISONIC;
   request.ICMND.msg6 = 0;
   if (configuration.isKeeLoqenabled()) request.ICMND.msg6 |= msg6_KeeLoq;
   if (configuration.isHomeConfortenabled()) request.ICMND.msg6 |= msg6_HC;

   return toBuffer(request, GET_RBUF_STRUCT_SIZE(ICMND));
}

shared::communication::CByteBuffer CTransceiver::buildStartReceiverCmd() const
{
   RBUF request;
   MEMCLEAR(request.ICMND); // For better performance, just clear the needed sub-structure of RBUF

   request.ICMND.packetlength = ENCODE_PACKET_LENGTH(ICMND);
   request.ICMND.packettype = pTypeInterfaceControl;
   request.ICMND.subtype = sTypeInterfaceCommand;
   request.ICMND.seqnbr = m_seqNumberProvider->next();
   request.ICMND.cmnd = cmdStartRec;

   return toBuffer(request, GET_RBUF_STRUCT_SIZE(ICMND));
}


boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CTransceiver::buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                                                     boost::shared_ptr<const yApi::IDeviceCommand>
                                                                                                     command) const
{
   try
   {
      const auto& deviceDetails = api->getDeviceDetails(command->getDevice());
      const auto deviceType = deviceDetails.get<unsigned char>("type");

      // Create the RFXCom message
      switch (deviceType)
      {
      case pTypeLighting1:
         return rfxcomMessages::CLighting1(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeLighting2:
         return rfxcomMessages::CLighting2(api, command, deviceDetails).encode(m_seqNumberProvider);
      case pTypeLighting3:
         return rfxcomMessages::CLighting3(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeLighting4:
         return rfxcomMessages::CLighting4(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeLighting5:
         return rfxcomMessages::CLighting5(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeLighting6:
         return rfxcomMessages::CLighting6(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeChime:
         return rfxcomMessages::CChime(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeCurtain:
         return rfxcomMessages::CCurtain1(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeFan:
         return rfxcomMessages::CFan(api, command->getKeyword(), command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeBlinds:
         return rfxcomMessages::CBlinds1(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeRFY:
         return rfxcomMessages::CRfy(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeHomeConfort:
         return rfxcomMessages::CHomeConfort(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeSecurity1:
         return rfxcomMessages::CSecurity1(api, command->getKeyword(), command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeSecurity2:
         return rfxcomMessages::CSecurity2(api, command->getKeyword(), command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeCamera:
         return rfxcomMessages::CCamera1(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeRemote:
         return rfxcomMessages::CRemote(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeThermostat1:
         return rfxcomMessages::CThermostat1(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeThermostat2:
         return rfxcomMessages::CThermostat2(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeThermostat3:
         return rfxcomMessages::CThermostat3(api, command->getKeyword(), command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeThermostat4:
         return rfxcomMessages::CThermostat4(api, command->getKeyword(), command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeRadiator1:
         return rfxcomMessages::CRadiator1(api, command->getKeyword(), command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      case pTypeFS20:
         return rfxcomMessages::CFS20(api, command->getBody(), deviceDetails).encode(m_seqNumberProvider);
      default:
         YADOMS_LOG(error) << "Invalid command \"" << command->getBody() << "\" : " << " unknown type " << deviceType;
         throw shared::exception::CInvalidParameter(command->getBody());
      }
   }
   catch (shared::exception::CException& e)
   {
      throw shared::exception::CInvalidParameter((boost::format("Invalid command \"%1%\" : %2%") % command->getBody() % e.what()).str());
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CTransceiver::buildRfyProgramMessage(
   const shared::communication::CByteBuffer& lastRequest) const
{
   return rfxcomMessages::CRfy::encodeProgramMessage(m_seqNumberProvider,
                                                     lastRequest);
}

boost::shared_ptr<rfxcomMessages::IRfxcomMessage> CTransceiver::decodeRfxcomMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                                    const shared::communication::CByteBuffer& data) const
{
   try
   {
      const auto buf = reinterpret_cast<const RBUF* const>(data.begin());
      const auto bufSize = data.size();

      boost::shared_ptr<rfxcomMessages::IRfxcomMessage> message;
      switch (buf->RXRESPONSE.packettype) //TODO Ajouter m_pairingHelper partout
      {
      case pTypeInterfaceMessage: message = boost::make_shared<rfxcomMessages::CTransceiverStatus>(*buf, bufSize, m_seqNumberProvider);
         break;
      case pTypeRecXmitMessage: message = boost::make_shared<rfxcomMessages::CAck>(*buf, bufSize, m_seqNumberProvider);
         break;
      case pTypeRFXMeter: message = boost::make_shared<rfxcomMessages::CRFXMeter>(api, *buf, bufSize);
         break;
      case pTypeLighting1: message = boost::make_shared<rfxcomMessages::CLighting1>(api, *buf, bufSize);
         break;
      case pTypeLighting2: message = boost::make_shared<rfxcomMessages::CLighting2>(api, *buf, bufSize);
         break;
      case pTypeLighting3: message = boost::make_shared<rfxcomMessages::CLighting3>(api, *buf, bufSize);
         break;
      case pTypeLighting4: message = boost::make_shared<rfxcomMessages::CLighting4>(api, *buf, bufSize, m_unsecuredProtocolFilters.at(pTypeLighting4), m_pairingHelper);
         break;
      case pTypeLighting5: message = boost::make_shared<rfxcomMessages::CLighting5>(api, *buf, bufSize, m_unsecuredProtocolFilters.at(pTypeLighting5));
         break;
      case pTypeLighting6: message = boost::make_shared<rfxcomMessages::CLighting6>(api, *buf, bufSize, m_pairingHelper);
         break;
      case pTypeChime: message = boost::make_shared<rfxcomMessages::CChime>(api, *buf, bufSize);
         break;
      case pTypeFan: message = boost::make_shared<rfxcomMessages::CFan>(api, *buf, bufSize);
         break;
      case pTypeCurtain: message = boost::make_shared<rfxcomMessages::CCurtain1>(api, *buf, bufSize);
         break;
      case pTypeBlinds: message = boost::make_shared<rfxcomMessages::CBlinds1>(api, *buf, bufSize);
         break;
      case pTypeRFY: message = boost::make_shared<rfxcomMessages::CRfy>(api, *buf, bufSize);
         break;
      case pTypeHomeConfort: message = boost::make_shared<rfxcomMessages::CHomeConfort>(api, *buf, bufSize);
         break;
      case pTypeTEMP_RAIN: message = boost::make_shared<rfxcomMessages::CTempRain>(api, *buf, bufSize);
         break;
      case pTypeTEMP: message = boost::make_shared<rfxcomMessages::CTemp>(api, *buf, bufSize);
         break;
      case pTypeHUM: message = boost::make_shared<rfxcomMessages::CHumidity>(api, *buf, bufSize);
         break;
      case pTypeTEMP_HUM: message = boost::make_shared<rfxcomMessages::CTempHumidity>(api, *buf, bufSize);
         break;
      case pTypeBARO: message = boost::make_shared<rfxcomMessages::CBarometric>(api, *buf, bufSize);
         break;
      case pTypeTEMP_HUM_BARO: message = boost::make_shared<rfxcomMessages::CTempHumidityBarometric>(api, *buf, bufSize);
         break;
      case pTypeRAIN: message = boost::make_shared<rfxcomMessages::CRain>(api, *buf, bufSize);
         break;
      case pTypeWIND: message = boost::make_shared<rfxcomMessages::CWind>(api, *buf, bufSize);
         break;
      case pTypeUV: message = boost::make_shared<rfxcomMessages::CUV>(api, *buf, bufSize);
         break;
      case pTypeDT: message = boost::make_shared<rfxcomMessages::CDateTime>(api, *buf, bufSize);
         break;
      case pTypeCURRENT: message = boost::make_shared<rfxcomMessages::CCurrent>(api, *buf, bufSize);
         break;
      case pTypeENERGY: message = boost::make_shared<rfxcomMessages::CEnergy>(api, *buf, bufSize);
         break;
      case pTypeCURRENTENERGY: message = boost::make_shared<rfxcomMessages::CCurrentEnergy>(api, *buf, bufSize, m_unsecuredProtocolFilters.at(pTypeCURRENTENERGY));
         break;
      case pTypePOWER: message = boost::make_shared<rfxcomMessages::CPower>(api, *buf, bufSize);
         break;
      case pTypeWEIGHT: message = boost::make_shared<rfxcomMessages::CWeight>(api, *buf, bufSize);
         break;
      case pTypeCARTELECTRONIC: message = boost::make_shared<rfxcomMessages::CCartelectronic>(api, *buf, bufSize);
         break;
      case pTypeRFXSensor: message = boost::make_shared<rfxcomMessages::CRFXSensor>(api, *buf, bufSize);
         break;
      case pTypeSecurity1: message = boost::make_shared<rfxcomMessages::CSecurity1>(api, *buf, bufSize, m_unsecuredProtocolFilters.at(pTypeSecurity1));
         break;
      case pTypeSecurity2: message = boost::make_shared<rfxcomMessages::CSecurity2>(api, *buf, bufSize);
         break;
      case pTypeCamera: message = boost::make_shared<rfxcomMessages::CCamera1>(api, *buf, bufSize);
         break;
      case pTypeRemote: message = boost::make_shared<rfxcomMessages::CRemote>(api, *buf, bufSize);
         break;
      case pTypeThermostat1: message = boost::make_shared<rfxcomMessages::CThermostat1>(api, *buf, bufSize);
         break;
      case pTypeThermostat2: message = boost::make_shared<rfxcomMessages::CThermostat2>(api, *buf, bufSize);
         break;
      case pTypeThermostat3: message = boost::make_shared<rfxcomMessages::CThermostat3>(api, *buf, bufSize);
         break;
      case pTypeRadiator1: message = boost::make_shared<rfxcomMessages::CRadiator1>(api, *buf, bufSize);
         break;
      case pTypeBBQ: message = boost::make_shared<rfxcomMessages::CBbq>(api, *buf, bufSize);
         break;
      case pTypeFS20: message = boost::make_shared<rfxcomMessages::CFS20>(api, *buf, bufSize);
         break;
      default:
         {
            YADOMS_LOG(warning) << "Invalid RfxCom message received, unknown packet type " << std::setfill('0')
               << std::setw(sizeof(unsigned char) * 2) << std::hex << static_cast<int>(buf->RXRESPONSE.packettype);
            return boost::shared_ptr<rfxcomMessages::IRfxcomMessage>();
         }
      }

      logMessage(api, message);
      return message;
   }
   catch (CMessageFilteredException& exception)
   {
      YADOMS_LOG(warning) << exception.what();
      YADOMS_LOG(warning) <<
         "Message received, but filtered as protocol is unsecured and can create false devices. Device should be seen often enough to get out of filter.";
      return boost::shared_ptr<rfxcomMessages::IRfxcomMessage>();
   }
   catch (std::exception& exception)
   {
      YADOMS_LOG(error) << "Invalid RfxCom message received : " << exception.what();
      return boost::shared_ptr<rfxcomMessages::IRfxcomMessage>();
   }
}

std::string CTransceiver::createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                               const yApi::IManuallyDeviceCreationData& data) const
{
   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> msg;
   try
   {
      const auto deviceType = data.getDeviceType();
      // Lighting1
      if (deviceType == "x10")
         msg = boost::make_shared<rfxcomMessages::CLighting1>(api, sTypeX10, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "ARC")
         msg = boost::make_shared<rfxcomMessages::CLighting1>(api, sTypeARC, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "ab400d")
         msg = boost::make_shared<rfxcomMessages::CLighting1>(api, sTypeAB400D, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "waveman")
         msg = boost::make_shared<rfxcomMessages::CLighting1>(api, sTypeWaveman, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "emw200")
         msg = boost::make_shared<rfxcomMessages::CLighting1>(api, sTypeEMW200, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "impuls")
         msg = boost::make_shared<rfxcomMessages::CLighting1>(api, sTypeIMPULS, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "risingSun")
         msg = boost::make_shared<rfxcomMessages::CLighting1>(api, sTypeRisingSun, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "philips")
         msg = boost::make_shared<rfxcomMessages::CLighting1>(api, sTypePhilips, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "energenie")
         msg = boost::make_shared<rfxcomMessages::CLighting1>(api, sTypeEnergenie, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "energenie5")
         msg = boost::make_shared<rfxcomMessages::CLighting1>(api, sTypeEnergenie5, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "gdr2")
         msg = boost::make_shared<rfxcomMessages::CLighting1>(api, sTypeGDR2, data.getDeviceName(), data.getConfiguration());

         // Lighting2
      else if (deviceType == "AC")
         msg = boost::make_shared<rfxcomMessages::CLighting2>(api, sTypeX10, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "homeEasyEU")
         msg = boost::make_shared<rfxcomMessages::CLighting2>(api, sTypeARC, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "anslut")
         msg = boost::make_shared<rfxcomMessages::CLighting2>(api, sTypeARC, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "kambrookRf3672")
         msg = boost::make_shared<rfxcomMessages::CLighting2>(api, sTypeARC, data.getDeviceName(), data.getConfiguration());

         // Lighting3
      else if (deviceType == "koppla")
         msg = boost::make_shared<rfxcomMessages::CLighting3>(api, sTypeKoppla, data.getDeviceName(), data.getConfiguration());

         // Lighting4
      else if (deviceType == "pt2262")
         msg = boost::make_shared<rfxcomMessages::CLighting4>(api, sTypePT2262, data.getDeviceName(), data.getConfiguration());

         // Lighting5
      else if (deviceType == "lightwaveRf")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeLightwaveRF, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "emw100")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeEMW100, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "bbsb")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeBBSB, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "mdRemote106")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeMDREMOTE, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "rsl")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeRSL, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "livolo")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeLivolo, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "trc02")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeTRC02, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "aoke")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeAoke, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "trc02_2")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeTRC02_2, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "eurodomest")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeEurodomest, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "livoloAppliance")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeLivoloAppliance, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "rgb432w")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeRGB432W, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "mdremote107")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeMDREMOTE107, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "legrandCad")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeLegrandCAD, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "mdRemote108")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeMDREMOTE108, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "kangtai")
         msg = boost::make_shared<rfxcomMessages::CLighting5>(api, sTypeKangtai, data.getDeviceName(), data.getConfiguration());

         // Lighting6
      else if (deviceType == "blyss")
         msg = boost::make_shared<rfxcomMessages::CLighting6>(api, sTypeBlyss, data.getDeviceName(), data.getConfiguration());

         // Chime
      else if (deviceType == "byronSx")
         msg = boost::make_shared<rfxcomMessages::CChime>(api, sTypeByronSX, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "byronMp001")
         msg = boost::make_shared<rfxcomMessages::CChime>(api, sTypeByronMP001, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "selectPlus")
         msg = boost::make_shared<rfxcomMessages::CChime>(api, sTypeSelectPlus, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "envivo")
         msg = boost::make_shared<rfxcomMessages::CChime>(api, sTypeEnvivo, data.getDeviceName(), data.getConfiguration());

         // Fan
      else if (deviceType == "siemensSf01")
         msg = boost::make_shared<rfxcomMessages::CFan>(api, sTypeSiemensSF01, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "lucciAir")
         msg = boost::make_shared<rfxcomMessages::CFan>(api, sTypeLucciAir, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "seavTxs4")
         msg = boost::make_shared<rfxcomMessages::CFan>(api, sTypeSeavTXS4, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "westinghouse7226640")
         msg = boost::make_shared<rfxcomMessages::CFan>(api, sTypeWestinghouse, data.getDeviceName(), data.getConfiguration());

         // Curtain1
      else if (deviceType == "harrisonCurtain")
         msg = boost::make_shared<rfxcomMessages::CCurtain1>(api, sTypeHarrison, data.getDeviceName(), data.getConfiguration());

         // Blinds1
      else if (deviceType == "rollerTrolHastaNew")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT0, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "hastaOld")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT1, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "aOkRf01")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT2, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "aOkAc114")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT3, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "raex")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT4, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "mediaMount")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT5, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "dc106")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT6, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "forest")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT7, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "chamberlaincs4330cn")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT8, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "sunperyBtx")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT9, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "dolatDlm1")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT10, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "asp")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT11, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "confexx")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT12, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "screenline")
         msg = boost::make_shared<rfxcomMessages::CBlinds1>(api, sTypeBlindsT13, data.getDeviceName(), data.getConfiguration());

         // Rfy
      else if (deviceType == "rfy")
         msg = boost::make_shared<rfxcomMessages::CRfy>(api, sTypeRFY, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "rfyExt")
         msg = boost::make_shared<rfxcomMessages::CRfy>(api, sTypeRFYext, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "asa")
         msg = boost::make_shared<rfxcomMessages::CRfy>(api, sTypeASA, data.getDeviceName(), data.getConfiguration());

         // HomeConfort
      else if (deviceType == "homeConfort")
         msg = boost::make_shared<rfxcomMessages::CHomeConfort>(api, sTypeHomeConfortTEL010, data.getDeviceName(), data.getConfiguration());

         // Security1
      else if (deviceType == "x10SecurityRemote")
         msg = boost::make_shared<rfxcomMessages::CSecurity1>(api, sTypeSecX10R, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "meiantech")
         msg = boost::make_shared<rfxcomMessages::CSecurity1>(api, sTypeMeiantech, data.getDeviceName(), data.getConfiguration());

         // Security2
      else if (deviceType == "keeLoq")
         msg = boost::make_shared<rfxcomMessages::CSecurity2>(api, sTypeSec2Classic, data.getDeviceName(), data.getConfiguration());

         // Camera1
      else if (deviceType == "cameraX10Ninja")
         msg = boost::make_shared<rfxcomMessages::CCamera1>(api, sTypeNinja, data.getDeviceName(), data.getConfiguration());

         // Thermostat1
      else if (deviceType == "digimax")
         msg = boost::make_shared<rfxcomMessages::CThermostat1>(api, sTypeDigimax, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "digimaxShort")
         msg = boost::make_shared<rfxcomMessages::CThermostat1>(api, sTypeDigimaxShort, data.getDeviceName(), data.getConfiguration());

         // Thermostat2
      else if (deviceType == "he105")
         msg = boost::make_shared<rfxcomMessages::CThermostat2>(api, sTypeHE105, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "rts10")
         msg = boost::make_shared<rfxcomMessages::CThermostat2>(api, sTypeRTS10, data.getDeviceName(), data.getConfiguration());

         // Thermostat3
      else if (deviceType == "g6rH4t1")
         msg = boost::make_shared<rfxcomMessages::CThermostat3>(api, sTypeMertikG6RH4T1, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "g6rH4tb")
         msg = boost::make_shared<rfxcomMessages::CThermostat3>(api, sTypeMertikG6RH4TB, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "g6rH4td")
         msg = boost::make_shared<rfxcomMessages::CThermostat3>(api, sTypeMertikG6RH4TD, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "g6rH4s")
         msg = boost::make_shared<rfxcomMessages::CThermostat3>(api, sTypeMertikG6RH4S, data.getDeviceName(), data.getConfiguration());

         // Thermostat4
      else if (deviceType == "mcz1PelletStove")
         msg = boost::make_shared<rfxcomMessages::CThermostat4>(api, sTypeMCZ1, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "mcz2PelletStove")
         msg = boost::make_shared<rfxcomMessages::CThermostat4>(api, sTypeMCZ2, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "mcz2PelletStove")
         msg = boost::make_shared<rfxcomMessages::CThermostat4>(api, sTypeMCZ3, data.getDeviceName(), data.getConfiguration());

         // Radiator1
      else if (deviceType == "smartwares")
         msg = boost::make_shared<rfxcomMessages::CRadiator1>(api, sTypeSmartwares, data.getDeviceName(), data.getConfiguration());

         // FS20
      else if (deviceType == "fs20")
         msg = boost::make_shared<rfxcomMessages::CFS20>(api, sTypeFS20, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "fht8v")
         msg = boost::make_shared<rfxcomMessages::CFS20>(api, sTypeFHT8V, data.getDeviceName(), data.getConfiguration());
      else if (deviceType == "fht80")
         msg = boost::make_shared<rfxcomMessages::CFS20>(api, sTypeFHT80, data.getDeviceName(), data.getConfiguration());

      else
         throw shared::exception::CInvalidParameter("Unknown device type");
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Fail to create device manually, invalid parameter : " << e.what();
      YADOMS_LOG(error) << "data : " << data.getConfiguration().serialize();
      throw CManuallyDeviceCreationException("invalid parameter");
   }
   catch (shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Fail to create device manually, out of range : " << e.what();
      YADOMS_LOG(error) << "data : " << data.getConfiguration().serialize();
      throw CManuallyDeviceCreationException("out of range");
   }

   return msg->getDeviceName();
}

void CTransceiver::logMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                              const boost::shared_ptr<rfxcomMessages::IRfxcomMessage>& message)
{
   if (!!message && !message->getDeviceName().empty())
   {
      YADOMS_LOG(information) << "Receive data for " << message->getDeviceName();
      for (const auto& keyword : message->keywords())
         YADOMS_LOG(information) << "  - " << keyword->getKeyword() << " : " << keyword->formatValue();
   }
}
