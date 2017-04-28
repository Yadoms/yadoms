#include "stdafx.h"
#include "Transceiver.h"
#include <shared/Log.h>
#include <shared/tools/Random.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

#define IS_PACKED
#include "rfplayerApi/usb_frame_api.h"

CTransceiver::CTransceiver()
{
}

CTransceiver::~CTransceiver()
{
}


std::string CTransceiver::buildHelloCmd() const
{
   return "ZIA++HELLO\r\n";
}


std::string CTransceiver::buildStartListeningData() const
{
   return "ZIA++FORMAT BINARY\r\n";
}

std::string CTransceiver::buildReceiverConfigurationCommand(const CConfiguration::CProtocols & protocols) const
{
   std::string disabledProtocols;

   if (!protocols.isX10Active())
      disabledProtocols += " X10";
   if (!protocols.isRTSActive())
      disabledProtocols += " RTS";
   if (!protocols.isVisonicActive())
      disabledProtocols += " VISONIC";
   if (!protocols.isBlyssActive())
      disabledProtocols += " BLYSS";
   if (!protocols.isChaconActive())
      disabledProtocols += " CHACON";
   if (!protocols.isOregonV1Active())
      disabledProtocols += " OREGONV1";
   if (!protocols.isOregonV2Active())
      disabledProtocols += " OREGONV2";
   if (!protocols.isOregonV3Active())
      disabledProtocols += " OREGONV3/OWL";
   if (!protocols.isDomiaActive())
      disabledProtocols += " DOMIA";
   if (!protocols.isX2DActive())
      disabledProtocols += " X2D";
   if (!protocols.isKD101Active())
      disabledProtocols += " KD101";
   if (!protocols.isParrotActive())
      disabledProtocols += " PARROT";

   std::string command = "REPEATER + *";  //all protocols
   if (!disabledProtocols.empty())
      command += " -" + disabledProtocols;
   command += "\r\n";
   return command;
}

std::string CTransceiver::buildRepeaterActivationCommand(bool repeaterActive) const
{
   if (repeaterActive)
      return "REPEATER ON\r\n";
   return "REPEATER OFF\r\n";
}


std::string CTransceiver::buildRepeaterConfigurationCommand(const CConfiguration::CProtocols & protocols) const
{
   std::string disabledProtocols;

   if (!protocols.isX10Active())
      disabledProtocols += " X10";
   if (!protocols.isRTSActive())
      disabledProtocols += " RTS";
   if (!protocols.isVisonicActive())
      disabledProtocols += " VISONIC";
   if (!protocols.isBlyssActive())
      disabledProtocols += " BLYSS";
   if (!protocols.isChaconActive())
      disabledProtocols += " CHACON";
   if (!protocols.isOregonV1Active())
      disabledProtocols += " OREGONV1";
   if (!protocols.isOregonV2Active())
      disabledProtocols += " OREGONV2";
   if (!protocols.isOregonV3Active())
      disabledProtocols += " OREGONV3/OWL";
   if (!protocols.isDomiaActive())
      disabledProtocols += " DOMIA";
   if (!protocols.isX2DActive())
      disabledProtocols += " X2D";
   if (!protocols.isKD101Active())
      disabledProtocols += " KD101";
   //parrot not supported by repeater

   std::string command = "REPEATER + *";  //all protocols
   if (!disabledProtocols.empty())
      command += " -" + disabledProtocols;
   command += "\r\n";
   return command;
}

std::string CTransceiver::buildLedActivityCommand(bool ledActivity) const
{
   if (ledActivity)
      return "LEDACTIVITY 1\r\n";
   return "LEDACTIVITY 0\r\n";
}


DECLARE_ENUM_HEADER(EBlyssCommands,
   ((Off)(0))
   ((On)(1))
   ((Dim)(2))
   ((AllOff)(4))
   ((AllOn)(5))
   ((Assoc)(6))
)


DECLARE_ENUM_IMPLEMENTATION(EBlyssCommands,
   ((Off)("OFF"))
   ((On)("ON/Alert"))
   ((Dim))
   ((AllOff))
   ((AllOn))
   ((Assoc))
)

class CBlyssCommandKeyword : public shared::plugin::yPluginApi::historization::CSingleHistorizableData<EBlyssCommands>
{
public:
   //-----------------------------------------------------
   ///\brief                     Constructor
   ///\param[in] keywordName     Yadoms keyword name
   //-----------------------------------------------------
   explicit CBlyssCommandKeyword(const std::string& keywordName);

   //-----------------------------------------------------
   ///\brief                     Destructor
   //-----------------------------------------------------
   virtual ~CBlyssCommandKeyword();
};


DECLARE_CAPACITY(BlyssCommandCapacity, "blyss_capacity", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kEnum);

CBlyssCommandKeyword::CBlyssCommandKeyword(const std::string& keywordName)
   : shared::plugin::yPluginApi::historization::CSingleHistorizableData<EBlyssCommands>(keywordName,
      BlyssCommandCapacity(),
      shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
{
}

CBlyssCommandKeyword::~CBlyssCommandKeyword()
{
}


std::string CTransceiver::createDeviceManually(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api, const shared::plugin::yPluginApi::IManuallyDeviceCreationData & request)
{
   if (api->getYadomsInformation()->developperMode())
   {
      YADOMS_LOG(information) << "Receive event kEventManuallyDeviceCreation : \n"
         << "\tDevice name : " << request.getDeviceName();

      request.getConfiguration().printToLog(YADOMS_LOG(information));
   }

   //determine the device name (as an example, it take the provided deviceName and append an hexadecimal value) to ensure devicename is unique
   std::string devId = (boost::format("%1%_0x%2$08X") % request.getDeviceName() % shared::tools::CRandom::generateNbBits(26, false)).str();
   if (request.getDeviceType() == "blyss")
   {
      YADOMS_LOG(information) << "Blyss config : groupCode : " << request.getConfiguration().get<std::string>("groupCode");
      YADOMS_LOG(information) << "Blyss config : unitCode : " << request.getConfiguration().get<int>("unitCode");

      //create device
      if (!api->deviceExists(devId))
      {
         auto obj = boost::make_shared<CBlyssCommandKeyword>("command");
         shared::CDataContainer details;
         details.set("frequency", "433");
         details.set("protocol", "blyss");
         api->declareDevice(devId, request.getDeviceType(), "BLYSS", obj, details);
      }


      //create keywords
      return devId;
   }

   return "";
}

std::string CTransceiver::generateCommand(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api, boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command)
{
   shared::CDataContainer details = api->getDeviceDetails(command->getDevice());
   shared::CDataContainer config = api->getDeviceConfiguration(command->getDevice());

   if (details.get<std::string>("protocol") == "blyss" && details.get<std::string>("frequency") == "433")
   {
      if(details.containsValue("id"))
         return (boost::format("ZIA++%1% BLYSS ID %2%\r\n") % command->getBody() % details.get<std::string>("id")).str();
      else
         return (boost::format("ZIA++%1% BLYSS %2%%3%\r\n") % command->getBody() % config.get<std::string>("groupCode") % config.get<int>("unitCode")).str();
   }
   return "";
}


struct REGULAR_OUTGOING_BINARY_USB_FRAME
{
   MESSAGE_CONTAINER_HEADER header;
   REGULAR_INCOMING_BINARY_USB_FRAME content;
};

union IDMAPPER
{
   unsigned int id;
   struct UNITID
   {
      unsigned char id1;
      unsigned char id2;
      unsigned char id3;
      unsigned char id4;
   } mapping;
};
shared::communication::CByteBuffer CTransceiver::generateCommandBinary(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api, boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command)
{
   REGULAR_OUTGOING_BINARY_USB_FRAME data;
   data.header.Sync1 = SYNC1_CONTAINER_CONSTANT;
   data.header.Sync2 = SYNC2_CONTAINER_CONSTANT;
   data.header.SourceDestQualifier = SOURCEDEST433_868;
   data.header.QualifierOrLen_lsb = sizeof(REGULAR_INCOMING_BINARY_USB_FRAME) & 0xFF;
   data.header.QualifierOrLen_msb = (sizeof(REGULAR_INCOMING_BINARY_USB_FRAME) & 0xFF00) >> 8;

   shared::CDataContainer details = api->getDeviceDetails(command->getDevice());
   shared::CDataContainer config = api->getDeviceConfiguration(command->getDevice());

   if (details.get<std::string>("protocol") == "blyss" && details.get<std::string>("frequency") == "433" && details.containsValue("id"))
   {
     
      unsigned int id = details.get<unsigned int>("id");
      data.content.frameType = 0;
      data.content.cluster = 0;
      data.content.protocol = SEND_BLYSS_PROTOCOL_433;
      data.content.action = EBlyssCommands::parse(command->getBody()).toInteger();

      IDMAPPER map;
      map.mapping.id1 = (unsigned char)((id & 0xFF000000) >> 24);
      map.mapping.id2 = (unsigned char)((id & 0x00FF0000) >> 16);
      map.mapping.id3 = (unsigned char)((id & 0x0000FF00) >> 8);
      map.mapping.id4 = (unsigned char)((id & 0x000000FF));

      data.content.ID = id;
      data.content.dimValue = 0;
      data.content.burst = 0;
      data.content.qualifier = 0;
      data.content.reserved2 = 0;
      /*
      res[0] = 0; //frame type
      res[1] = 0; //cluster
      res[2] = 12; //protocol (blyss_433 = 12)
      res[3] =  //action
      res[6] = (id & 0x000000FF); //lsb
      res[7] = ((id & 0x0000FF00) >> 8);
      res[4] = ((id & 0x00FF0000) >> 16);
      res[5] = ((id & 0xFF000000) >> 24); //msb
      res[8] = 0; //dim = 0
      res[9] = 0; //burst = 0
      res[10] = 0; //qualifier = 0
      res[11] = 0; //reserved= 0
      */
   }

   shared::communication::CByteBuffer res((const unsigned char*)&data, sizeof(REGULAR_OUTGOING_BINARY_USB_FRAME));
   return res;
}