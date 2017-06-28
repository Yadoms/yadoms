#include "stdafx.h"
#include "ManuallyDeviceFactory.h"
#include <shared/Log.h>
#include "specificHistorizers/outgoing/Blyss.h"
#include "specificHistorizers/outgoing/Visonic.h"
#include "specificHistorizers/outgoing/X10.h"
#include "specificHistorizers/outgoing/Domia.h"
#include "specificHistorizers/outgoing/Kd101.h"
#include "specificHistorizers/outgoing/Parrot.h"
#include "specificHistorizers/outgoing/X2D.h"
#include "specificHistorizers/outgoing/Rts.h"


void CManuallyDeviceFactory::createDeviceManually(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api, const shared::plugin::yPluginApi::IManuallyDeviceCreationData& request)
{
   if (api->getYadomsInformation()->developperMode())
   {
      YADOMS_LOG(information) << "Receive event kEventManuallyDeviceCreation : \n"
         << "\tDevice name : " << request.getDeviceName();

      request.getConfiguration().printToLog(YADOMS_LOG(information));
   }

   if (request.getDeviceType() == "blyss")
   {
      YADOMS_LOG(information) << "Blyss config : groupCode : " << request.getConfiguration().get<std::string>("groupCode");
      YADOMS_LOG(information) << "Blyss config : unitCode : " << request.getConfiguration().get<int>("unitCode");

      auto keyword = boost::make_shared<specificHistorizers::outgoing::CBlyssKeyword>("command");
      shared::CDataContainer details;
      details.set("frequency", "433");
      details.set("protocol", "blyss");
      details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
      if (api->getDeviceModel(request.getDeviceName()).empty())
         api->updateDeviceModel(request.getDeviceName(), "BLYSS");
      api->updateDeviceDetails(request.getDeviceName(), details);
      api->declareKeyword(request.getDeviceName(), keyword);

      return;
   }
   if (request.getDeviceType() == "x10")
   {
      auto keyword = boost::make_shared<specificHistorizers::outgoing::CX10Keyword>("command");
      shared::CDataContainer details;
      details.set("id", request.getConfiguration().get<int>("id"));
      details.set("frequency", "433");
      details.set("protocol", "x10");
      details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
      if (api->getDeviceModel(request.getDeviceName()).empty())
         api->updateDeviceModel(request.getDeviceName(), "X10");
      api->updateDeviceDetails(request.getDeviceName(), details);
      api->declareKeyword(request.getDeviceName(), keyword);

      return;
   }
   if (request.getDeviceType() == "domia")
   {
      auto keyword = boost::make_shared<specificHistorizers::outgoing::CDomiaKeyword>("command");
      shared::CDataContainer details;
      details.set("id", request.getConfiguration().get<int>("id"));
      details.set("frequency", "433");
      details.set("protocol", "domia");
      details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
      if (api->getDeviceModel(request.getDeviceName()).empty())
         api->updateDeviceModel(request.getDeviceName(), "DOMIA");
      api->updateDeviceDetails(request.getDeviceName(), details);
      api->declareKeyword(request.getDeviceName(), keyword);

      return;
   }
   if (request.getDeviceType() == "kd101")
   {
      auto keyword = boost::make_shared<specificHistorizers::outgoing::CKd101Keyword>("command");
      shared::CDataContainer details;
      details.set("id", request.getConfiguration().get<int>("id"));
      details.set("frequency", "433");
      details.set("protocol", "kd101");
      details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
      if (api->getDeviceModel(request.getDeviceName()).empty())
         api->updateDeviceModel(request.getDeviceName(), "KD101");
      api->updateDeviceDetails(request.getDeviceName(), details);
      api->declareKeyword(request.getDeviceName(), keyword);

      return;
   }
   if (request.getDeviceType() == "parrot")
   {
      auto keyword = boost::make_shared<specificHistorizers::outgoing::CParrotKeyword>("command");
      shared::CDataContainer details;
      details.set("id", request.getConfiguration().get<int>("id"));
      details.set("protocol", "parrot");
      details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
      if (api->getDeviceModel(request.getDeviceName()).empty())
         api->updateDeviceModel(request.getDeviceName(), "PARROT");
      api->updateDeviceDetails(request.getDeviceName(), details);
      api->declareKeyword(request.getDeviceName(), keyword);

      return;
   }
   if (request.getDeviceType() == "visonic")
   {
      auto keyword = boost::make_shared<specificHistorizers::outgoing::CVisonicKeyword>("command");
      shared::CDataContainer details;
      details.set("id", request.getConfiguration().get<int>("id"));
      details.set("frequency", request.getConfiguration().get<int>("frequency"));
      details.set("protocol", "visonic");
      details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
      details.set("qualifier", request.getConfiguration().getWithDefault<std::string>("qualifier", ""));
      if (api->getDeviceModel(request.getDeviceName()).empty())
         api->updateDeviceModel(request.getDeviceName(), "VISONIC");
      api->updateDeviceDetails(request.getDeviceName(), details);
      api->declareKeyword(request.getDeviceName(), keyword);

      return;
   }
   if (request.getDeviceType() == "x2dAlarm433" || request.getDeviceType() == "x2dAlarm866" || request.getDeviceType() == "x2dShutter" || request.getDeviceType() == "x2dElec" || request.getDeviceType() == "x2dGas")
   {
      auto keyword = boost::make_shared<specificHistorizers::outgoing::CX2DKeyword>("command");
      shared::CDataContainer details;
      details.set("id", request.getConfiguration().get<int>("id"));

      if (request.getDeviceType() == "x2dAlarm433")
         details.set("frequency", 433);
      else
         details.set("frequency", 868);

      details.set("protocol", "x2d");
      details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
      if (api->getDeviceModel(request.getDeviceName()).empty())
         api->updateDeviceModel(request.getDeviceName(), "X2D");
      api->updateDeviceDetails(request.getDeviceName(), details);
      api->declareKeyword(request.getDeviceName(), keyword);

      return;
   }
   if (request.getDeviceType() == "rts")
   {
      auto keyword = boost::make_shared<specificHistorizers::outgoing::CRtsKeyword>("command");
      shared::CDataContainer details;
      details.set("id", request.getConfiguration().get<int>("id"));
      details.set("frequency", 433);
      details.set("protocol", "rts");
      details.set("burst", request.getConfiguration().getWithDefault<std::string>("burst", ""));
      details.set("qualifier", request.getConfiguration().getWithDefault<std::string>("qualifier", ""));
      if (api->getDeviceModel(request.getDeviceName()).empty())
         api->updateDeviceModel(request.getDeviceName(), "RTS");
      api->updateDeviceDetails(request.getDeviceName(), details);
      api->declareKeyword(request.getDeviceName(), keyword);
      return;
   }
}

