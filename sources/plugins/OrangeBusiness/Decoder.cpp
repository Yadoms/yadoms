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
                                    boost::shared_ptr<shared::CDataContainer>& message)
{
	std::map<std::string, boost::shared_ptr<equipments::IEquipment>> equipmentList;
   message->printToLog(YADOMS_LOG(trace));

   auto errorCode = message->getWithDefault<std::string>("code","");

   if (errorCode == "UNAUTHORIZED")
      throw CUnauthorizedException(message->get<std::string>("message"));

   if (!errorCode.empty())
      throw shared::exception::CException(message->get<std::string>("message"));

   auto equipments = message->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>("data");

   for (const auto& equipmentIterator : equipments)
   {
      auto name = equipmentIterator->get<std::string>("name");
      auto devEUI = equipmentIterator->get<std::string>("devEUI");
      auto newEquipment(boost::make_shared<equipments::CDefaultEquipment>(name, devEUI, api));
	   equipmentList.insert(std::pair<std::string, boost::shared_ptr<equipments::IEquipment>>(name, newEquipment));
      YADOMS_LOG(information) << "create device name = " << name << " devEUI = " << devEUI;
   }

   return equipmentList;
}

bool CDecoder::isFrameComplete(boost::shared_ptr<shared::CDataContainer>& message)
{
   int page = message->get<int>("page");
   int pageSize = message->get<int>("size");

   if (message->get<int>("totalCount") > (page * pageSize))
      return true;
   else
      return false;
}

boost::shared_ptr<shared::CDataContainer> CDecoder::getLastData(boost::shared_ptr<shared::CDataContainer>& response)
{
   auto messages = response->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>("");
   boost::shared_ptr<shared::CDataContainer> lastData = shared::CDataContainer::make();

   if (!messages.empty())
   {
      // first message is the last message received
      auto message = *messages.begin();

      //Copy to the answer needed information
      lastData->set("id", message->get<std::string>("id"));
      lastData->set("timestamp", message->get<std::string>("timestamp"));
      lastData->set("payload", message->get<std::string>("value.payload"));
      lastData->set("rssi", message->get<double>("metadata.network.lora.rssi"));
      lastData->set("snr", message->get<double>("metadata.network.lora.snr"));
      lastData->set("signalLevel", message->get<int>("value.signalLevel"));
   }
   return lastData;
}