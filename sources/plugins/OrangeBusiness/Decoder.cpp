#include "stdafx.h"
#include "Decoder.h"
#include <shared/Log.h>
#include "DefaultEquipment.h"
#include "UnauthorizedException.hpp"

CDecoder::CDecoder()
{}

CDecoder::~CDecoder()
{}

std::map<std::string, boost::shared_ptr<equipments::IEquipment>> CDecoder::decodeDevicesMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                    shared::CDataContainer& message)
{
	std::map<std::string, boost::shared_ptr<equipments::IEquipment>> equipmentList;
   message.printToLog(YADOMS_LOG(trace));

   auto errorcode = message.getWithDefault<std::string>("code","");

   if (errorcode == "UNAUTHORIZED")
      throw CUnauthorizedException(message.get<std::string>("message"));

   if (errorcode != "")
      throw shared::exception::CException(message.get<std::string>("message"));

   auto equipments = message.get<std::vector<shared::CDataContainer> >("data");
   std::vector<shared::CDataContainer>::iterator equipmentIterator;

   for (equipmentIterator = equipments.begin(); equipmentIterator != equipments.end(); ++equipmentIterator)
   {
      std::string name = (*equipmentIterator).get<std::string>("name");
      std::string devEUI = (*equipmentIterator).get<std::string>("devEUI");
      boost::shared_ptr<equipments::CDefaultEquipment> newEquipment(boost::make_shared<equipments::CDefaultEquipment>(name, devEUI, api));
	   equipmentList.insert(std::pair<std::string, boost::shared_ptr<equipments::IEquipment>>(name, newEquipment));
      YADOMS_LOG(information) << "create device name = " << name << " devEUI = " << devEUI;
   }

   return equipmentList;
}

bool CDecoder::isFrameComplete(shared::CDataContainer& message)
{
   int page = message.get<int>("page");
   int pageSize = message.get<int>("size");

   if (message.get<int>("totalCount") > (page * pageSize))
      return true;
   else
      return false;
}

shared::CDataContainer CDecoder::getLastData(shared::CDataContainer& response)
{
   auto messages = response.get<std::vector<shared::CDataContainer> >("");
   shared::CDataContainer lastData;

   if (messages.size() > 0)
   {
      // first message is the last message received
      auto message = messages[0];

      //Copy to the answer needed information
      lastData.set("id", message.get<std::string>("id"));
      lastData.set("timestamp", message.get<std::string>("timestamp"));
      lastData.set("payload", message.get<std::string>("value.payload"));
      lastData.set("rssi", message.get<double>("metadata.network.lora.rssi"));
      lastData.set("snr", message.get<double>("metadata.network.lora.snr"));
      lastData.set("signalLevel", message.get<int>("value.signalLevel"));
   }
   return lastData;
}