#include "stdafx.h"
#include "ManuallyDeviceFactory.h"
#include <shared/Log.h>
#include <shared/tools/Random.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include "specificHistorizers/outgoing/Blyss.h"
#include "specificHistorizers/outgoing/Visonic.h"
#include "specificHistorizers/outgoing/X10.h"
#include "specificHistorizers/outgoing/Domia.h"
#include "specificHistorizers/outgoing/Kd101.h"
#include "specificHistorizers/outgoing/Parrot.h"
#include "specificHistorizers/outgoing/X2D.h"
#include "specificHistorizers/outgoing/Rts.h"


const std::string CManuallyDeviceFactory::createDeviceManually(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api, const shared::plugin::yPluginApi::IManuallyDeviceCreationData & request)
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
         auto obj = boost::make_shared<specificHistorizers::outgoing::CBlyssKeyword>("command");
         shared::CDataContainer details;
         details.set("frequency", "433");
         details.set("protocol", "blyss");
         details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
         api->declareDevice(devId, request.getDeviceType(), "BLYSS", obj, details);
      }

      //create keywords
      return devId;
   } 
   else if (request.getDeviceType() == "x10")
   {
      //create device
      if (!api->deviceExists(devId))
      {
         auto obj = boost::make_shared<specificHistorizers::outgoing::CX10Keyword>("command");
         shared::CDataContainer details;
         details.set("id", request.getConfiguration().get<int>("id"));
         details.set("frequency", "433");
         details.set("protocol", "x10");
         details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
         api->declareDevice(devId, request.getDeviceType(), "X10", obj, details);
      }

      //create keywords
      return devId;
   }   
   else if (request.getDeviceType() == "domia")
   {
      //create device
      if (!api->deviceExists(devId))
      {
         auto obj = boost::make_shared<specificHistorizers::outgoing::CDomiaKeyword>("command");
         shared::CDataContainer details;
         details.set("id", request.getConfiguration().get<int>("id"));
         details.set("frequency", "433");
         details.set("protocol", "domia");
         details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
         api->declareDevice(devId, request.getDeviceType(), "DOMIA", obj, details);
      }

      //create keywords
      return devId;
   }   
   else if (request.getDeviceType() == "kd101")
   {
      //create device
      if (!api->deviceExists(devId))
      {
         auto obj = boost::make_shared<specificHistorizers::outgoing::CKd101Keyword>("command");
         shared::CDataContainer details;
         details.set("id", request.getConfiguration().get<int>("id"));
         details.set("frequency", "433");
         details.set("protocol", "kd101");
         details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
         api->declareDevice(devId, request.getDeviceType(), "KD101", obj, details);
      }

      //create keywords
      return devId;
   } 
   else if (request.getDeviceType() == "parrot")
   {
      //create device
      if (!api->deviceExists(devId))
      {
         auto obj = boost::make_shared<specificHistorizers::outgoing::CParrotKeyword>("command");
         shared::CDataContainer details;
         details.set("id", request.getConfiguration().get<int>("id"));
         details.set("protocol", "parrot");
         details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
         api->declareDevice(devId, request.getDeviceType(), "PARROT", obj, details);
      }

      //create keywords
      return devId;
   }
   else if (request.getDeviceType() == "visonic")
   {
      //create device
      if (!api->deviceExists(devId))
      {
         auto obj = boost::make_shared<specificHistorizers::outgoing::CVisonicKeyword>("command");
         shared::CDataContainer details;
         details.set("id", request.getConfiguration().get<int>("id"));
         details.set("frequency", request.getConfiguration().get<int>("frequency"));
         details.set("protocol", "visonic");
         details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
         details.set("qualifier", request.getConfiguration().getWithDefault<std::string>("qualifier", ""));
         api->declareDevice(devId, request.getDeviceType(), "VISONIC", obj, details);
      }

      //create keywords
      return devId;
   }  
   else if (request.getDeviceType() == "x2dAlarm433" || request.getDeviceType() == "x2dAlarm866" || request.getDeviceType() == "x2dShutter" || request.getDeviceType() == "x2dElec" || request.getDeviceType() == "x2dGas")
   {
      //create device
      if (!api->deviceExists(devId))
      {
         auto obj = boost::make_shared<specificHistorizers::outgoing::CX2DKeyword>("command");
         shared::CDataContainer details;
         details.set("id", request.getConfiguration().get<int>("id"));

         if (request.getDeviceType() == "x2dAlarm433")
            details.set("frequency", 433);
         else
            details.set("frequency", 868);

         details.set("protocol", "x2d");
         details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
         api->declareDevice(devId, request.getDeviceType(), "X2D", obj, details);
      }

      //create keywords
      return devId;
   }
   else if (request.getDeviceType() == "rts")
   {
      //create device
      if (!api->deviceExists(devId))
      {
         auto obj = boost::make_shared<specificHistorizers::outgoing::CRtsKeyword>("command");
         shared::CDataContainer details;
         details.set("id", request.getConfiguration().get<int>("id"));
         details.set("frequency", 433);
         details.set("protocol", "rts");
         details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
         details.set("qualifier", request.getConfiguration().getWithDefault<std::string>("qualifier", ""));
         api->declareDevice(devId, request.getDeviceType(), "RTS", obj, details);
      }

      //create keywords
      return devId;
   }
   return "";
}
